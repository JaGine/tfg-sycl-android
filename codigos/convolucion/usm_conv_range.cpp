#include <chrono>
#include <iostream>

#include <CL/sycl.hpp>

using data_type = unsigned short;

using namespace cl::sycl;

unsigned int img_width = 1920;
unsigned int img_height = 1080;
unsigned int num_pixels = img_width*img_height; // 12484800
unsigned int img_size = num_pixels * 3;

// KERNELS DE CONVOLUCION DISPONIBLES

static float filter_gauss_factor = 256.0f;
static float filter_gauss[25] = { 
  1.0f,   4.0f,   7.0f,   4.0f,   1.0f,
  4.0f,   16.0f,  24.0f,  16.0f,  4.0f,
  7.0f,   24.0f,  36.0f,  24.0f,  7.0f, 
  4.0f,   16.0f,  24.0f,  16.0f,  4.0f,
  1.0f,   4.0f,   7.0f,   4.0f,   1.0f };
static const int filter_gauss_width = 5;

static float filter_unsharp_masking_factor = -256.0f;
static float filter_unsharp_masking[25] = { 
  1.0f,   4.0f,    7.0f,   4.0f,   1.0f,
  4.0f,   16.0f,   24.0f,  16.0f,  4.0f,
  7.0f,   24.0f, -476.0f,  24.0f,  7.0f, 
  4.0f,   16.0f,   24.0f,  16.0f,  4.0f,
  1.0f,   4.0f,    7.0f,   4.0f,   1.0f };
static const int filter_unsharp_masking_width = 5;


static float filter_sharpen_factor = 1.0f;
static float filter_sharpen[9] = { 
  -1.0f,  -1.0f,  -1.0f,
  -1.0f,   9.5f,  -1.0f,
  -1.0f,  -1.0f,  -1.0f };
static const int filter_sharpen_width = 3;

static float filter_box_blur_factor = 9.0f;
static float filter_box_blur[9] = { 
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f };
static const int filter_box_blur_width = 3;

static float filter_edge_detection_factor = 1.0f;
static float filter_edge_detection[9] = { 
   0.0f,  -1.0f,   0.0f,
  -1.0f,   4.0f,  -1.0f,
   0.0f,  -1.0f,   0.0f };
static const int filter_edge_detection_width = 3;

static void ReportTime(const std::string &msg, event e) {
  unsigned long time_start =
      e.get_profiling_info<info::event_profiling::command_start>();

  unsigned long time_end =
      e.get_profiling_info<info::event_profiling::command_end>();

  double elapsed = (time_end - time_start) / 1e6;
  std::cout << msg << elapsed << " milliseconds\n";
}

void convolution(cl::sycl::queue& q,
                           const data_type* acc_in,
                           data_type* acc_out,
                           float* acc_filter,
                           const int filter_width_aux)
{

  cl::sycl::range<1> work_items{num_pixels};
  event e1;

  {
    auto t1 = std::chrono::steady_clock::now();   // Start timing

    e1 = q.submit([&](cl::sycl::handler& cgh){

      unsigned int num_cols = img_width * 3, num_rows = img_height;
      int filter_width = filter_width_aux;

      cgh.parallel_for(work_items, [=] (cl::sycl::id<1> tid) {

        int i = tid * 3;
        int row = i/num_cols;
        int col = i%num_cols;

        float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
        int half_filter_width = filter_width / 2;

        for(int k = 0; k < filter_width; k++) {
          for (int l = 0; l < filter_width; l++) {
            int row_aux = row + (k - half_filter_width);
            int col_aux = col + ((l - half_filter_width) * 3);

            // Make sure the filter doesn't go out of the row and column range.
            row_aux = (row_aux < 0) ? 0 : row_aux;
            row_aux = (row_aux >= num_rows) ? num_rows-1 : row_aux;
            col_aux = (col_aux < 0) ? 0 : col_aux;
            col_aux = (col_aux >= num_cols) ? num_cols-3 : col_aux;

            // Calculating the sum of the filter applied to the image
            sumR += acc_in[ (row_aux * num_cols) + col_aux     ] * acc_filter[(k * filter_width) + l];
            sumG += acc_in[ (row_aux * num_cols) + col_aux + 1 ] * acc_filter[(k * filter_width) + l];
            sumB += acc_in[ (row_aux * num_cols) + col_aux + 2 ] * acc_filter[(k * filter_width) + l];
          }
        }
        sumR = sumR < 0 ? 0 : sumR;
        sumG = sumG < 0 ? 0 : sumG;
        sumB = sumB < 0 ? 0 : sumB;
        acc_out[i  ] = sumR > 255 ? 255 : sumR; // dst_image[(row*num_cols) + col]
        acc_out[i+1] = sumG > 255 ? 255 : sumG; // dst_image[(row*num_cols) + col + 1]
        acc_out[i+2] = sumB > 255 ? 255 : sumB; // dst_image[(row*num_cols) + col + 2]
      });
    });
    q.wait_and_throw();

    auto t2 = std::chrono::steady_clock::now();   // Stop timing

    std::cout << "Convolution execution time from host: " <<
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << " microseconds" << std::endl;
  }

  //ReportTime("Convolution kernel time: ", e1);
}

int main(int argc, char* argv[])
{
  if(argc == 3) {
    img_width = atoi(argv[1]);
    img_height = atoi(argv[2]);

    num_pixels = img_width*img_height;
    img_size = num_pixels * 3;
  }

  std::cout << img_width << "x" << img_height << ": " << num_pixels << std::endl; 


  cl::sycl::queue q {
    default_selector_v, 
    property::queue::enable_profiling{}};




  // ASIGNING THE FILTER
  float filter_factor = filter_sharpen_factor;
  float* filter = filter_sharpen;
  const int filter_width = filter_sharpen_width;
  for(int i = 0; i < filter_width*filter_width; i++) {
    filter[i] = filter[i] / filter_factor;
  }

  data_type *image_in = new data_type[img_size];
  // init dummy input image
  data_type val = 0;
  for(int i = 0; i < img_size; i++) {
    image_in[i] = val;
    val++;
    if(val > 255) val = 0;
  }

  data_type *acc_in = malloc_shared<data_type>(img_size, q);
  data_type *acc_out = malloc_shared<data_type>(img_size, q);
  int filter_size = filter_width*filter_width;
  float *acc_filter = malloc_shared<float>(filter_size, q);

  if ((acc_in == nullptr) || (acc_out == nullptr) || (acc_filter == nullptr)) {
    if (acc_in != nullptr) free(acc_in, q);
    if (acc_out != nullptr) free(acc_out, q);
    if (acc_filter != nullptr) free(acc_filter, q);
    std::cout << "Shared memory allocation failure.\n";
    return -1;
  }

  std::memcpy(acc_in, image_in, img_size);
  
  for(int i = 0; i < filter_width*filter_width; i++) {
      acc_filter[i] = filter[i];
  }
  for(int i = 0; i < filter_width*filter_width; i++) {
      if(filter[i] != acc_filter[i]){
          cout << "Error al copiar el filtro\n";
          return -1;
      }
  }

  // execute kernel
  convolution(q, acc_in, acc_out, acc_filter, filter_width);

  
  // for(int i = 0; i < 20; i++){
  //   std::cout << image_out[i] << std::endl;
  // }

  delete[] image_in;
  free(acc_in, q);
  free(acc_out, q);
  free(acc_filter, q);

  return 0;
}

