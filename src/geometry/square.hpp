#include "geometry.hpp"

class Square: public Geometry {
public:
    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos, ray & inRay);
    int getType();
};