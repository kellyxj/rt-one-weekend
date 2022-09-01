#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "geometry.hpp"

class Sphere: public Geometry {
public:
    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos);
    int getType();
};

#endif