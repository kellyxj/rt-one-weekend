#ifndef __COLOR_H__
#define __COLOR_H__

class color {
public:
    double r, g, b, a;
    color(): r(0), g(0), b(0), a(1) {}
    color(double red, double green, double blue): r(red), g(green), b(blue), a(1) {}
    color(double red, double green, double blue, double alpha): r(red), g(green), b(blue), a(alpha) {}
};

#endif