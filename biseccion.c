#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f1(double x, int caso);
int iteraciones(double a, double b, int caso, double epsilon);
double** biseccion(double a, double b, int num_iteraciones, int caso);


int main() {
    double a, b, epsilon;
    int num_iteraciones, opcion, decimales;

    printf("Seleccione la funcion para resolver con el metodo de Biseccion:\n");
    printf("1. f(x) = x^3 - x - 1\n");
    printf("2. f(x) = sin(x) - x^2\n");
    printf("3. f(x) = ln(x) - 1\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &opcion);
    fflush(stdin);
    printf("Dame el intervalo a,b (separados por espacio): ");
    scanf("%lf,%lf", &a, &b);
    fflush(stdin);
    printf("Dame la cantidad de decimales: ");
    scanf("%d", &decimales);
    fflush(stdin);
    epsilon = pow(10, -decimales);
    num_iteraciones = iteraciones(a, b, opcion, epsilon);
    if (num_iteraciones == -1) {
        return 1;
    }

    printf("Numero de iteraciones: %d\n", num_iteraciones);

    double** resultado = biseccion(a, b, num_iteraciones, opcion);

    printf("Iter\t a\t\t b\t\t c\t\t f(c)\n");
    for (int i = 0; i < num_iteraciones; i++) {
        printf("%d\t%lf\t%lf\t%lf\t%lf\n",
               (int)resultado[i][0], resultado[i][1], resultado[i][2], resultado[i][3], resultado[i][4]);
    }

    for (int i = 0; i < num_iteraciones; i++) {
        free(resultado[i]);
    }
    free(resultado);

    return 0;
}

double f1(double x, int caso) {
    switch (caso) {
        case 1: return pow(x, 3) - x - 1;
        case 2: return sin(x) - pow(x, 2);
        case 3: return log(x) - 1;
        default: return 0;
    }
}

int iteraciones(double a, double b, int caso, double epsilon) {
    if (f1(a, caso) * f1(b, caso) > 0) {
        fprintf(stderr, "Error: El intervalo es invalido.\n");
        return -1;
    } else {
        return (int)(ceil(log((b - a) / epsilon) / log(2)));
    }
}


double** biseccion(double a, double b, int num_iteraciones, int caso) {

    double** matriz = (double**)malloc(num_iteraciones * sizeof(double*));
    for (int i = 0; i < num_iteraciones; i++) {
        matriz[i] = (double*)malloc(5 * sizeof(double));
    }

    double c;
    for (int i = 0; i < num_iteraciones; i++) {
        c = (a + b) / 2.0;
        matriz[i][0] = i + 1;
        matriz[i][1] = a;
        matriz[i][2] = b;
        matriz[i][3] = c;
        matriz[i][4] = f1(c, caso);

        if (f1(a, caso) * f1(c, caso) > 0) {
            a = c;
        } else {
            b = c;
        }
    }

    return matriz;
}
