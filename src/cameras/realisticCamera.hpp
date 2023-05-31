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
        LensElementInterface(float curvatureRadius, float thickness, float eta, float apertureDiameter)
        : curvatureRadius(curvatureRadius/1000.0), thickness(thickness/1000.0), eta(eta), apertureRadius(apertureDiameter/2000.0) {};
    };

    std::vector<LensElementInterface> elementInterfaces;

    // Note: input diagonal size in mm!
    RealisticCamera(std::vector<LensElementInterface> lenses, vec4 eye, float pan, float tilt, int w, int h, float diagonal, float focusDistance)
    : elementInterfaces(lenses), eyePoint(eye), panAngle(pan), tiltAngle(tilt), diagonal(diagonal/1000.0), focusDistance(focusDistance) {
        setLookDirection(pan, tilt);
        width = w;
        height = h;
        computeProperties();

        computeExitPupilBounds(64);
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
    float focusDistance;
    Bounds2f bounds;

    std::vector<Bounds2f> exitPupilBounds;

    // Compute properties of the camera like its aspect ratio, physical bounds, and lens z-bounds
    void computeProperties();

    // Compute the z depths of the focal point and the principal plane for the given rays
    // Note that it assumes the rays are in camera space but returns z values in lens space...
    void computeCardinalPoints(ray &rIn, ray &rOut, float *pz, float *fz);

    // Computes both pairs of cardinal points for the lens system
    void computeThickLensApproximation(float pz[2], float fz[2]);

    // Focuses the lens system using the thick lens approximation (adjusts the rear-element's z-value)
    float focusThickLens(float focusDistance);

    // Compute the bounds of the exit pupil along radial positions away from the center of the sensor
    void computeExitPupilBounds(int nSamples);

    // Computes the bounds for the exit pupil for distance x0 to x1 away from the center of the sensor
    Bounds2f boundExitPupil(float pFilmX0, float pFilmX1) const;

    // Computes intersections with element in turn, terminating the ray and returning
    // false if its path is blocked along the way through the lens system. Otherwise, 
    // it returns true and initializes *outRay with the ray exiting the camera.
    bool traceLensesFromSensor (ray &rLens, ray &rOut);

    // Specialized sphere-ray intersection method for spheres centered along z axis
    bool intersectSphericalElement(float radius, float zCenter, ray rLens, float *t, vec4 *n);
};

#endif