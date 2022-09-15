#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../defines.hpp"
#include "../vec4.hpp"
#include "../color.hpp"
#include "../ray.hpp"
#include <cmath>

class Material {
public:
    virtual Color getColor(vec4 & pos) = 0;
    virtual ray scatter(ray & inRay, vec4 & pos, vec4 & normal) = 0;
};

#endif