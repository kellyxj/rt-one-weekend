#include "geometry.hpp"

class Light: public Geometry {
public:
    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos);
    vec4 sample(vec4 & origin);
};