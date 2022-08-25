#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>

class mat4 {
public:
    double entries [16];
    mat4(): entries([1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1]) {}

    mat4 transpose();
    mat4 invert();

    double det();

    //return this*b
    mat4 multiply(mat4 b);

    mat4 translate(vec4 v);
    mat4 scale(double a);
    mat4 rotate(double angle, vec4 axis);

}

#endif