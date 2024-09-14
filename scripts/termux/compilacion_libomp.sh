cd $HOME/AdaptiveCpp
mkdir build-libonly && cd build-libonly

# se selecciona clang por defecto
cmake \
    -DCMAKE_INSTALL_PREFIX=~/AdaptiveCpp-install-libonly \
    -DWITH_SSCP_COMPILER=OFF \
    -DWITH_STDPAR_COMPILER=OFF \
    -DWITH_ACCELERATED_CPU=OFF \
    -DWITH_CUDA_BACKEND=OFF \
    -DWITH_ROCM_BACKEND=OFF \
    -DWITH_OPENCL_BACKEND=OFF \
    -DWITH_LEVEL_ZERO_BACKEND=OFF \
    ..

make
# Da error. Tenemos que modificar el archivo AdaptiveCpp/src/runtime/omp/omp_allocator.cpp
# En la línea 62 cambiamos "std::aligned_alloc" por "malloc"
echo "Utilzar: nano ../sr/runtime/omp/omp_allocator.cpp"
echo "Cambiar la linea 62 para que en vez de ser:"
echo "std::aligned_alloc(<param1>, <param2>);"
echo "Sea:"
echo "malloc(<param2>);"

echo "Una vez cambiado, hacer: make install"
#make install
# tenemos instalado acpp (library-only) en el directorio $HOME/AdaptiveCpp-install-libonly
