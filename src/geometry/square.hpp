#include "geometry.hpp"

class Square: public Geometry {
public:
    GeometryType type = GeometryType::square;

    Hit trace(ray & inRay);
    vec4 getNormal(vec4 & pos, ray & inRay);

    json serialize();
    Geometry* deserialize(json json_);
};