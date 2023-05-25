#ifndef __REALISTIC_CAMERA_H__
#define __REALISTIC_CAMERA_H__

#include "camera.hpp"
#include "../math/mat4.hpp"

class RealisticCamera : public Camera {

public:

    struct LensElementInterface {
        // radius of the spherical element
        float curvatureRadius;

        // distance along Z to the next element to the right
        float thickness;

        // eta = index of refraction of the medium to the right of the interface
        float eta;

        // The element's symmetric extent away from the z-axis
        float apertureRadius;

        // Values are input as mm but stored as m!
        LensElementInterface(float curvatureRadius, float thickness, float eta, float apertureRadius)
        : curvatureRadius(curvatureRadius/1000.0), thickness(thickness/1000.0), eta(eta), apertureRadius(apertureRadius/1000.0) {};
    };

    std::vector<LensElementInterface> elementInterfaces;

    // Note: input diagonal size in mm!
    RealisticCamera(std::vector<LensElementInterface> lenses, vec4 eye, float pan, float tilt, int w, int h, float diagonal)
    : elementInterfaces(lenses), eyePoint(eye), panAngle(pan), tiltAngle(tilt), diagonal(diagonal/1000.0) {
        setLookDirection(pan, tilt);
        width = w;
        height = h;
        computeProperties();

    }

    void setEyePosition(vec4 pos);
    void setLookDirection(float pan, float tilt);

    // Return the origin and direction of the ray that has been ray-traced through the lens system
    ray getEyeRay(float xPos, float yPos);

    void setUVN();

    json serialize();

    Camera* deserialize(json json_);


private:

    vec4 eyePoint;
    vec4 aimPoint;
    vec4 up;
    mat4 camToWorldMatrix;

    float panAngle;
    float tiltAngle;

    vec4 uAxis;
    vec4 vAxis;
    vec4 nAxis;

    float diagonal;
    float aspect;
    float lensRearZ;
    float lensFrontZ;
    float rearElementRadius;
    Bounds2f bounds;

    // Compute properties of the camera like its aspect ratio, physical bounds, and lens z-bounds
    void computeProperties();

    // Computes intersections with element in turn, terminating the ray and returning
    // false if its path is blocked along the way through the lens system. Otherwise, 
    // it returns true and initializes *outRay with the ray exiting the camera.
    bool traceLensesFromSensor (ray &rLens, ray &rOut);

    // Specialized sphere-ray intersection method for spheres centered along z axis
    bool intersectSphericalElement(float radius, float zCenter, ray rLens, float *t, vec4 *n);
};

#endif