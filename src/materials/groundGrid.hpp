#include "material.hpp"

class groundGrid: public Material {
public:
    Color getColor(vec4 & pos);
};