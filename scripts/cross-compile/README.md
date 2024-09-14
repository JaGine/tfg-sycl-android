# Scripts para hacer una compilación cruzada de AdaptiveCpp

## Desde Ubuntu 22.04 (teniendo los paquetes y NDK instalados)

### Clonar este repositorio
```console
~ $ git clone https://github.com/JaGine/tfg-sycl-android.git
```

### Para compilar AdaptiveCpp omp.library-only

Cambiar los valores que se indican en el script [**cross_compile_adaptivecpp_libomp.sh**](cross_compile_adaptivecpp_libomp.sh) a los correspondientes antes de ejecutar el script.

```console
~ $ git clone https://github.com/AdaptiveCpp/AdaptiveCpp.git
~ $ sh tfg-sycl-android/scripts/cross-compile/cross_compile_adaptivecpp_libomp.sh
```
