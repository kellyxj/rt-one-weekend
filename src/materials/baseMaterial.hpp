#include "material.hpp"

class base: public Material {
public:
    Color c;
    Color getColor(vec4 & pos);
};