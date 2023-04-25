#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include "../math/vec4.hpp"
#include "../math/ray.hpp"
#include "../defines.hpp"

class Camera {
public:
    virtual ray getEyeRay(float xPos, float yPos) = 0;
    virtual json serialize() = 0;

    virtual ~Camera() {}

    int width;
    int height;

    float pixelWidth;
    float pixelHeight;

    float gamma;
    float exposure = 1;
};

#endif