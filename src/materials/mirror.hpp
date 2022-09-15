#include "material.hpp"

class Mirror: public Material {
public:
    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
};