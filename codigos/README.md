# Códigos utilizados para la medición de tiempos

En [color](color) se encuentran los filtros de color.

En [convolucion](convolucion) se encuentran los filtros de convolución.

## Para compilar y utilizar los códigos

Antes de compilar los códigos es necesario cambiar el kernel de convolución que se quiere utilizar manualmente (línea 74 (buffers) o línea 153 (usm) para los codigos de convolución).

### Compilar con AdaptiveCpp
```console
~ $ acpp <codigo-a-compilar> -O3 -o <nombre-ejecutable>
```

### Compilar con DPC++ (en Linux)
```console
~ $ icpx -fsycl <codigo-a-compilar> -o <nombre-ejecutable>
```

---

### Ejecución de los códigos

#### Ejecutar (para range)
```console
~ $ ./<nombre-ejecutable> <anchura-imagen> <altura-imagen>
```

#### Ejecutar (para nd_range)
```console
~ $ ./<nombre-ejecutable> <anchura-imagen> <altura-imagen> <local-range>
```
El valor del parámetro **local-range** utilizado para convolución 1D ha sido 32, 64, 128 y 256.

Para convolución 2D **local-range** ha sido 8 y 16 (8x8 y 16x16).

---

#### Ejemplo de compilación y ejecución para nd_range (utilizando un kernel de convolución 3x3)
```console
~ $ acpp conv_ndrange_2d -O3 -o conv3_ndrange_2d
~ $ ./conv3_ndrange_2d 1920 1080 8
```
