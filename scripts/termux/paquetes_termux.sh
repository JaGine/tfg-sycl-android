# instalamos los requisitos principales para compilar AdaptiveCpp
pkg i -y python3
pkg i -y boost
pkg i -y cmake

# instalamos todo lo relacionado con LLVM/Clang
pkg i -y clang
pkg i -y llvm
pkg i -y libcompiler-rt
pkg i -y libllvm
pkg i -y libllvm-static
pkg i -y libpolly
pkg i -y lld
pkg i -y llvm-tools
pkg i -y llvm-gold
pkg i -y mlir

#activamos el acceso a almacenamiento para copiar los siguientes archivos
termux-setup-storage

cp /vendor/lib64/egl/mt6789/libGLES_mali.so ~/../usr/opt/vendor/lib/
cp /vendor/lib64/libged.so ~/../usr/opt/vendor/lib/
cp /vendor/lib64/mt6789/libpq_cust_base.so ~/../usr/opt/vendor/lib/

# instalamos lo relacionado con opencl
pkg i -y clinfo

# instalamos los siguientes paquetes para usar OpenCL
pkg i -y ocl-icd
pkg i -y opencl-headers
pkg i -y opencl-vendor-driver
pkg i -y opencl-clhpp

pkg i -y spirv-headers
pkg i -y spirv-tools

echo "Listo!"