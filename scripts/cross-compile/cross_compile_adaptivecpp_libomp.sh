#cambiar NDK a el diretorio indicado correcto
export NDK=
export NDK_BIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/

#cambiar DIR_INSTALL al directorio que queramos la instalación
export DIR_INSTALL=~/AdaptiveCpp-libonly-android

#cambiar el numero de api (default 34) a la version correspondiente
export ANDROID_API_NUM=android-34

cmake \
    -DCMAKE_C_COMPILER=$NDK_BIN/aarch64-linux-android34-clang \
    -DCMAKE_CXX_COMPILER=$NDK_BIN/aarch64-linux-android34-clang++ \
    -DCMAKE_INSTALL_PREFIX=$DIR_INSTALL \
    -DWITH_SSCP_COMPILER=OFF \
    -DWITH_STDPAR_COMPILER=OFF \
    -DWITH_CUDA_BACKEND=OFF \
    -DWITH_ACCELERATED_CPU=OFF \
    -DWITH_OPENCL_BACKEND=OFF \
    -DWITH_LEVEL_ZERO_BACKEND=OFF \
    -DWITH_ROCM_BACKEND=OFF \
    -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=$ANDROID_API_NUM \
    ..

make install
