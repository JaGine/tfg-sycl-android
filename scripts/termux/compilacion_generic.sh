cd $HOME/AdaptiveCpp
mkdir build-generic && cd build-generic

cmake \
    -DCMAKE_INSTALL_PREFIX=~/AdaptiveCpp-install-generic \
    -DWITH_SSCP_COMPILER=ON \
    -DWITH_STDPAR_COMPILER=OFF \
    -DWITH_ACCELERATED_CPU=OFF \
    -DWITH_CUDA_BACKEND=OFF \
    -DWITH_ROCM_BACKEND=OFF \
    -DWITH_OPENCL_BACKEND=ON \
    -DOpenCL_LIBRARY=~/../usr/opt/vendor/lib/libOpenCL.so \
    -DWITH_LEVEL_ZERO_BACKEND=OFF \
    ..

make

# error en el archivo AdaptiveCpp/src/libkernel/sscp/host/math.cpp
# exp10, fmaxmagf, fmaxmag no declarados. 
echo "exp10, fmaxmagf, fmaxmag no declarados"
echo "Para arreglar: nano ../src/libkernel/sscp/host/math.cpp"
echo "Y comentar las lineas correspondientes"
echo "Una vez corregido, hacer: make install"
