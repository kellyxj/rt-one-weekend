#include "sphere.hpp"

class SphereLight: public Sphere{
public:
    double brightness;
    bool isLight = true;
    //vec4 sample();
};