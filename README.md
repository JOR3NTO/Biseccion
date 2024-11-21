## **Metodo de Biseccion usando JNI**

### *Creación paso a paso de Bibliotecas Dinámicas con JNI*

### Código en Java para la Integración JNI

Primero, crea la carpeta principal del proyecto, por ejemplo: `mkdir biseccion`. A continuación, crea una subcarpeta para el código en Java: `mkdir libbiseccion`. Dentro de esta subcarpeta, desarrolla el archivo `.java` con el comando `nano JAvaBiseccion.java`. El código en Java debería ser el siguiente:

```java
package libbiseccion;

public class JavaBiseccion {

    // Métodos nativos
    public native double[][] biseccion(double a, double b, int numIteraciones, int caso);
    public native int iteraciones(double a, double b, int caso, double epsilon);

}
```

### Código en C

Después de finalizar el código en Java, genera el archivo `.h` utilizando: `javac JavaBiseccion.java -h .`. Para verificar el contenido del archivo compilado, puedes usar el comando: `file JavaBiseccion.class`.

Duplica el archivo de encabezado de la siguiente manera: `cp libbiseccion_JavaBiseccion.h libbiseccion_JavaBiseccion.c`, y luego edita el archivo `.c` usando `nano libbiseccion_JavaBiseccion.c`. El código en C debería ser el siguiente:

```c
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libbiseccion_JavaBiseccion.h"

double f1(double x, int caso) {
    switch (caso) {
        case 1: return pow(x, 3) - x - 1;
        case 2: return sin(x) - pow(x, 2);
        case 3: return log(x) - 1;
        default: return 0;
    }
}

JNIEXPORT jobjectArray JNICALL Java_libbiseccion_JavaBiseccion_biseccion
  (JNIEnv *env, jobject obj, jdouble a, jdouble b, jint num_iteraciones, jint caso) {
    jclass doubleArrayClass = (*env)->FindClass(env, "[D");
    jobjectArray result = (*env)->NewObjectArray(env, num_iteraciones, doubleArrayClass, NULL);

    for (int i = 0; i < num_iteraciones; i++) {
        double c = (a + b) / 2.0;
        jdouble temp[5] = {i + 1, a, b, c, f1(c, caso)};
        jdoubleArray row = (*env)->NewDoubleArray(env, 5);
        (*env)->SetDoubleArrayRegion(env, row, 0, 5, temp);
        (*env)->SetObjectArrayElement(env, result, i, row);

        if (f1(a, caso) * f1(c, caso) > 0) {
            a = c;
        } else {
            b = c;
        }
    }
    return result;
}

JNIEXPORT jint JNICALL Java_libbiseccion_JavaBiseccion_iteraciones
  (JNIEnv *env, jobject obj, jdouble a, jdouble b, jint caso, jdouble epsilon) {
    if (f1(a, caso) * f1(b, caso) > 0) {
        return -1;
    }
    return (jint)(ceil(log((b - a) / epsilon) / log(2)));
}


```

### Generación de la Biblioteca

Para compilar la biblioteca, utiliza el siguiente comando: `gcc -c -fPIE libbiseccion_JavaBiseccion.c -I/usr/lib/jvm/java-1.17.0-openjdk-amd64/include/ -I/usr/lib/jvm/java-1.17.0-openjdk-amd64/include/linux/ -o libbiseccion.o`. Luego, genera la biblioteca compartida ejecutando: `gcc -shared libbiseccion.o -o libbiseccion.so`. Para finalizar la instalación de la biblioteca, ejecuta estos comandos:

- `sudo cp libbiseccion.so /usr/lib`
- `sudo cp libbisecion_JavaBiseccion.h /usr/include`
- `sudo ldconfig`

### Envío de Datos y Salida de Resultados en Java

Regresa a la carpeta principal `biseccion`, crea una nueva subcarpeta `mkdir gestorJNI`, y dentro de `gestorJNI`, crea el archivo `nano JavaJNIGestorApp.java` con el siguiente código:

```java
package gestorJNI;

import libbiseccion.JavaBiseccion;

public class JavaJNIGestorApp {
    static {
        System.loadLibrary("biseccion");
    }

    public native double[][] biseccion(double a, double b, int numIteraciones, int caso);
    public native int iteraciones(double a, double b, int caso, double epsilon);

    public static void main(String[] args) {
        JavaBiseccion metodo = new JavaBiseccion();

        double a = 1.0, b = 2.0;
        double epsilon = 1e-6;
        int caso = 1; // f(x) = x^3 - x - 1

        int numIteraciones = metodo.iteraciones(a, b, caso, epsilon);
        System.out.println("Número de iteraciones necesarias: " + numIteraciones);

        double[][] resultados = metodo.biseccion(a, b, numIteraciones, caso);

        System.out.println("Iter\t a\t\t b\t\t c\t\t f(c)");
        for (double[] fila : resultados) {
            System.out.printf("%d\t%f\t%f\t%f\t%f%n", (int) fila[0], fila[1], fila[2], fila[3], fil>
        }
    }
}


```
Al terminar el desarrollo del código, compílalo desde la carpeta principal con:

- `javac gestorJNI/JavaJNIGestorApp.java`
- `java gestorJNI.JavaJNIGestorApp`

### Resultado Final

Al ejecutar el programa, obtendrás una salida similar a:

```
Número de iteraciones necesarias: 20
Iter     a               b               c               f(c)
1       1.000000        2.000000        1.500000        0.875000
2       1.000000        1.500000        1.250000        -0.296875
3       1.250000        1.500000        1.375000        0.224609
4       1.250000        1.375000        1.312500        -0.051514
5       1.312500        1.375000        1.343750        0.082611
6       1.312500        1.343750        1.328125        0.014576
7       1.312500        1.328125        1.320313        -0.018711
8       1.320313        1.328125        1.324219        -0.002128
9       1.324219        1.328125        1.326172        0.006209
10      1.324219        1.326172        1.325195        0.002037
11      1.324219        1.325195        1.324707        -0.000047
12      1.324707        1.325195        1.324951        0.000995
13      1.324707        1.324951        1.324829        0.000474
14      1.324707        1.324829        1.324768        0.000214
15      1.324707        1.324768        1.324738        0.000084
16      1.324707        1.324738        1.324722        0.000018
17      1.324707        1.324722        1.324715        -0.000014
18      1.324715        1.324722        1.324718        0.000002
19      1.324715        1.324718        1.324717        -0.000006
20      1.324717        1.324718        1.324718        -0.000002
```
