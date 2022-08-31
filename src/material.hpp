#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "vec4.hpp"
#include "color.hpp"
#include <cmath>

class Material {
public:
    virtual color getColor(vec4 & pos) = 0;
};

#endif