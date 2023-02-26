#include "light.hpp"

class RectangleLight : public Light
{
public:
    LightType type = LightType::rectangle;
    // always centered at the origin
    // can be transformed since light inherits from geometry
    float width = 0;
    float height = 0;

    RectangleLight();
    RectangleLight(float w, float h);

    float area();

    vec4 getPointOnLight();
    Hit trace(ray &inRay);
    vec4 getNormal(vec4 &pos, ray &inRay);

    json serialize();
    Light *deserialize(json json_);
};