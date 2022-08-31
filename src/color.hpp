#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>

class color {
public:
    double r, g, b, a;
    color(): r(0), g(0), b(0), a(1) {}
    color(double red, double green, double blue): r(red), g(green), b(blue), a(1) {}
    color(double red, double green, double blue, double alpha): r(red), g(green), b(blue), a(alpha) {}
};

inline std::ostream& operator << (std::ostream &os, const color &c) {
    os << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return os;
}

const color black(0,0,0);
const color white(1,1,1);

const color red(1,0,0);
const color green(0,1,0);
const color blue(0,0,1);

#endif