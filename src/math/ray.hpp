#ifndef __RAY_H__
#define __RAY_H__
#include "vec4.hpp"
#include "../color.hpp"
#include "../materials/indexOfRefraction.hpp"

class ray {
public:
    vec4 origin;
    vec4 direction;
    Color color;
    IOR n_i;

    ray(): origin(vec4()), direction(vec4()) {
        n_i.spectrum = white;
        color = black;
    }
    ray(vec4 o, vec4 dir): origin(o){
        n_i.spectrum = white;
        color = black;
        direction = dir.normalize();
    }
};

inline std::ostream& operator << (std::ostream &os, const ray &r) {
    os << "origin: " << r.origin << "\n";
    os << "direction: " << r.direction <<"\n";
    return os;
}

#endif