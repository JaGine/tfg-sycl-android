#include <chrono>
#include <iostream>

#include <CL/sycl.hpp>

using data_type = unsigned short;

using namespace cl::sycl;

unsigned int img_width = 1920;
unsigned int img_height = 1080;
unsigned int num_pixels = img_width*img_height; // 12484800
unsigned int img_size = num_pixels * 3;
unsigned int local_size = 8;

static void ReportTime(const std::string &msg, event e) {
  unsigned long time_start =
      e.get_profiling_info<info::event_profiling::command_start>();

  unsigned long time_end =
      e.get_profiling_info<info::event_profiling::command_end>();

  double elapsed = (time_end - time_start) / 1e6;
  std::cout << msg << elapsed << " milliseconds\n";
}

void sepia(cl::sycl::queue& q,
                           const data_type* in,
                           data_type* out)
{
  event e1;

  range<2> global_range( img_width, img_height );
  range<2> local_range(local_size, local_size);

  {


  unsigned int num_cols = img_width * 3;
  unsigned int img_height_aux = img_height;

    cl::sycl::buffer<data_type> buff_in(in, range(img_size));
    cl::sycl::buffer<data_type> buff_out(out, range(img_size));

    auto t1 = std::chrono::steady_clock::now();   // Start timing

    e1 = q.submit([&](cl::sycl::handler& cgh){
      auto acc_in = buff_in.get_access<cl::sycl::access::mode::read>(cgh);
      auto acc_out = buff_out.get_access<cl::sycl::access::mode::write>(cgh);

      cgh.parallel_for(nd_range<2>(global_range, local_range), [=] (cl::sycl::nd_item<2> item) {

        id<2> global_idx = item.get_global_id();
        //int row = global_idx[0];
        //int col = global_idx[1]*3;
        
        int i = (global_idx[0] * num_cols) + (global_idx[1] * 3);
        float temp;
        
        if(global_idx[0] < img_height_aux && global_idx[1] < num_cols){
          temp = (0.393f * acc_in[i]) + (0.769f * acc_in[i + 1]) +
               (0.189f * acc_in[i + 2]);
          acc_out[i] = temp > 255 ? 255 : temp;
          temp = (0.349f * acc_in[i]) + (0.686f * acc_in[i + 1]) +
                (0.168f * acc_in[i + 2]);
          acc_out[i + 1] = temp > 255 ? 255 : temp;
          temp = (0.272f * acc_in[i]) + (0.534f * acc_in[i + 1]) +
                (0.131f * acc_in[i + 2]);
          acc_out[i + 2] = temp > 255 ? 255 : temp;
        }
        
        
      });
    });
    q.wait_and_throw();

    auto t2 = std::chrono::steady_clock::now();   // Stop timing

    std::cout << "Color execution time from host: " <<
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << " microseconds" << std::endl;
  }

  //ReportTime("Color kernel time: ", e1);
}

int main(int argc, char* argv[])
{
  if(argc == 4) {
    img_width = atoi(argv[1]);
    img_height = atoi(argv[2]);
    local_size = atoi(argv[3]);

    num_pixels = img_width*img_height;
    img_size = num_pixels * 3;
  }

  std::cout << img_width << "x" << img_height << ": " << num_pixels << std::endl; 
  std::cout << "Local Size: " << local_size << "x" << local_size << std::endl; 

  cl::sycl::queue q {
    default_selector_v, 
    property::queue::enable_profiling{}};

  data_type *image_in = new data_type[img_size];
  data_type *image_out = new data_type[img_size];

  memset(image_out, 0, img_size * sizeof(data_type));

  // init dummy input image
  data_type val = 0;
  for(int i = 0; i < img_size; i++) {
    image_in[i] = val;
    val++;
    if(val > 255) val = 0;
  }

  // execute kernel
  sepia(q, image_in, image_out);

  // for(int i = 0; i < 20; i++){
  //   std::cout << image_out[i] << std::endl;
  // }

  delete[] image_in;
  delete[] image_out;

  return 0;
}

