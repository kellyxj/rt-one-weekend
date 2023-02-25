#include "math/ray.hpp"
#include "math/mat4.hpp"
#include "../hit.hpp"

class Light {
public:
    virtual vec4 getPointOnLight() = 0;
    virtual Hit trace(ray & inRay) = 0;
};