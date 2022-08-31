#include "geometry.hpp"

class Sphere: public Geometry {
public:
    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos);
    int getType();
};