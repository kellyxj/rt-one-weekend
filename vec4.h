class vec4 {
    double x, y, z, w;
    vec4(): x(0), y(0), z(0), w(0) {};
    vec4(double a, double b, double c): x(a), y(b), z(c), w(0) {};
    vec4(double a, double b, double c, double d): x(a), y(b), z(c), w(d) {};
    vec4 normalize();
};