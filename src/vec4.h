#pragma once

class vec4 {
public:
    double x, y, z, w;
    vec4(): x(0), y(0), z(0), w(0) {}
    vec4(double a, double b, double c): x(a), y(b), z(c), w(0) {}
    vec4(double a, double b, double c, double d): x(a), y(b), z(c), w(d) {}

    vec4 normalize();
    vec4 cross(vec4 &v);
    double dot(vec4 &v);
    double length();

    vec4 operator + (vec4 & v);
    vec4 & operator += (vec4 & v);
    vec4 operator - (vec4 & v);
    vec4 & operator -= (vec4 & v);
    vec4 operator * (double c);
    vec4 & operator *= (double c);
    vec4 operator / (double c);
    vec4 & operator /= (double c);
};