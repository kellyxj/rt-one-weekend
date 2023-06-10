#include "geometry.hpp"
#include "../materials/groundGrid.hpp"
#include "../materials/groundCheckerboard.hpp"

class Plane : public Geometry {
public:
    GeometryType type = GeometryType::plane;

    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos, ray & inRay);

    json serialize();
};