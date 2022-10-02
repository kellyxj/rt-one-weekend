#include "material.hpp"

class Glass: public Material {
public:
    double n_i;
    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
};