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
// que debe retornar       
        return -1;
    }
    return (jint)(ceil(log((b - a) / epsilon) / log(2)));
}
