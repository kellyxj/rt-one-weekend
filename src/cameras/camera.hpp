#ifndef __TRANSLATE_ANIMATION_H__
#define __TRANSLATE_ANIMATION_H__

#include "../animation/translateAnimation.hpp"

#endif

#ifndef __ROTATE_ANIMATION_H__
#define __ROTATE_ANIMATION_H__

#include "../animation/rotateAnimation.hpp"

#endif

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include "../math/mat4.hpp"
#include "../math/vec4.hpp"
#include "../math/ray.hpp"
#include "../defines.hpp"


class Camera {
public:
    virtual ray getEyeRay(float xPos, float yPos) = 0;
    virtual json serialize() = 0;

    virtual ~Camera() {}

    vec4 eyePoint;
    vec4 aimPoint;

    int width;
    int height;

    float pixelWidth;
    float pixelHeight;

    float gamma;
    float exposure = 1;

    std::vector<Animation*> animationList;

    //virtual void setEyePosition(vec4 pos);
    //virtual void setLookDirection(float pan, float tilt);
    virtual void setUVN() = 0;

    virtual void matSetEyePosition(mat4 transform) = 0;

    virtual vec4 getEyePoint() = 0;
};

#endif