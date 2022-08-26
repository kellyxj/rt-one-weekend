#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>
#include <ostream>
#include "vec4.h"
#define EPSILON 0.000000001

class mat4 {
public:
    double entries [16];
    mat4() {
        setIdentity();
    }

    mat4 transpose();
    mat4 invert();

    double det();

    //return this*b
    mat4 multiply(const mat4 & b);

    mat4 translate(const vec4 & v);
    mat4 scale(double a);
    mat4 rotate(double angle, const vec4 & axis);

    double &operator[] (int index) {
        return entries[index];
    }

    void setIdentity() {
        for(int i = 0; i < 16; i++) {
            entries[i] = 0;
        }
        entries[0] = 1;
        entries[5] = 1;
        entries[10] = 1;
        entries[15] = 1;
    }
};

inline std::ostream& operator << (std::ostream &os, const mat4 &a) {
    os << "[" << a.entries[0] << ", " << a.entries[1] << ", " << a.entries[2] << ", " << a.entries[3] << "\n";
    os << a.entries[4] << ", " << a.entries[5] << ", " << a.entries[6] << ", " << a.entries[7] << "\n";
    os << a.entries[8] << ", " << a.entries[9] << ", " << a.entries[10] << ", " << a.entries[11] << "\n";
    os << a.entries[12] << ", " << a.entries[13] << ", " << a.entries[14] << ", " << a.entries[15] << "]";
    
    return os;
}

#endif