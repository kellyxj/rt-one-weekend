#include "material.hpp"

class base: public Material {
public:
    MaterialType type = MaterialType::diffuse;

    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);

    json serialize();
    Material * deserialize(json json_);
};