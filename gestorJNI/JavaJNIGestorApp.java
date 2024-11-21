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
            System.out.printf("%d\t%f\t%f\t%f\t%f%n", (int) fila[0], fila[1], fila[2], fila[3], fila[4]);
        }
    }
}
