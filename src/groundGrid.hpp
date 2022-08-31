#include "material.hpp"

class groundGrid: public Material {
public:
    color getColor(vec4 & pos);
};