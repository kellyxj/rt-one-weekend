#include "geometry.hpp"
#include "groundGrid.hpp"

class Plane : public Geometry {
public:

    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos);
    int getType();
};