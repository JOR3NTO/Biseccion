package libbiseccion;

public class JavaBiseccion {

    // Métodos nativos
    public native double[][] biseccion(double a, double b, int numIteraciones, int caso);
    public native int iteraciones(double a, double b, int caso, double epsilon);

}
