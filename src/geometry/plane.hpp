#include "geometry.hpp"
#include "../materials/groundGrid.hpp"

class Plane : public Geometry {
public:
    GeometryType type = GeometryType::plane;

    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos, ray & inRay);

    json serialize();
    Geometry* deserialize(json json_);
};