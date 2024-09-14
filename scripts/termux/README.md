# Scripts de instalación y compilación para Termux

## Si no se tiene instalado Termux

### Para instalar Termux en nuestro dispositivo es necesario seguir los siguientes pasos:
1. Descargar el APK (en el dispositivo Android) que se encuentra disponible en la [página oficial de F-Droid](https://f-droid.org/es/packages/com.termux/).
2. Ejecutar dicho APK en el dispositivo, confirmar y esperar a su instalación.

---

---

## Una vez instalado Termux y dentro de Termux (seguir pasos)

### Para descargar los paquetes necesarios de Termux:
```console
~ $ sh tfg-sycl-android/scripts/termux/paquetes_termux.sh
```

---

### Para clonar este repositorio:
```console
~ $ git clone https://github.com/JaGine/tfg-sycl-android.git
```

---

### [Para clonar AdaptiveCpp (en $HOME) (usar uno de los siguientes)](clonar_adaptivecpp.sh)
```console
~ $ sh tfg-sycl-android/scripts/termux/clonar_adaptivecpp.sh
```
ó
```console
~ $ git clone https://github.com/AdaptiveCpp/AdaptiveCpp.git
```

### En caso de querer cambiar el commit de AdaptiveCpp a los utilizados en la memoria

#### Commit utilizado para testear omp.library-only en el apartado Resultados experimentales
```console
~ $ cd AdaptiveCpp
~/AdaptiveCpp $ git checkout c0add99698c0b5ed8b2865392d22995b1298f98b
```

#### Commit utilizado donde se compiló correctamente AdaptiveCpp en modo genérico
```console
~ $ cd AdaptiveCpp
~/AdaptiveCpp $ git checkout d57b9f5879c620cf5b13a3842337715f9e8015e6
```

---

### Para compilar AdaptiveCpp (en orden)

#### omp.library-only (cambiar los archivos necesarios, indicados por el [script](compilacion_libomp.sh))
```console
~ $ sh tfg-sycl-android/scripts/termux/compilacion_libomp.sh
```

#### generic (cambiar los archivos necesarios, indicados por el [script](compilacion_generic.sh))
```console
~ $ sh tfg-sycl-android/scripts/termux/compilacion_generic.sh
```

#### omp.accelerated
```console
~ $ sh tfg-sycl-android/scripts/termux/compilacion_accelerated.sh
```

---

### Para compilar y ejecutar los [ejemplos](../../codigos) utilizando AdaptiveCpp omp.library-only

#### compilación
```console
~ $ cd tfg-sycl-android/codigos
~/tfg-sycl-android/codigos $ ~/AdaptiveCpp-install-libonly/bin/acpp <color/convolucion>/<código-a-compilar> -O3 -o <nombre-ejecutable>
```

#### ejecución

Para range
```console
~/tfg-sycl-android/codigos $ <nombre-ejecutable> <anchura-imagen> <altura-imagen>
```

Para nd_range
```console
~/tfg-sycl-android/codigos $ <nombre-ejecutable> <anchura-imagen> <altura-imagen> <local-range>
```