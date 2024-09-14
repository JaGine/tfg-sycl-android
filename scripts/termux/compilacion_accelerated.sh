cd $HOME/AdaptiveCpp
mkdir build-acc && cd build-acc
# se selecciona clang por defecto
cmake \
    -DCMAKE_INSTALL_PREFIX=~/AdaptiveCpp-install-acc \
    -DWITH_SSCP_COMPILER=OFF \
    -DWITH_STDPAR_COMPILER=OFF \
    -DWITH_ACCELERATED_CPU=ON \
    -DWITH_CUDA_BACKEND=OFF \
    -DWITH_ROCM_BACKEND=OFF \
    -DWITH_OPENCL_BACKEND=OFF \
    -DWITH_LEVEL_ZERO_BACKEND=OFF \
    ..

make install
# tenemos instalado acpp (omp.accelerated)
