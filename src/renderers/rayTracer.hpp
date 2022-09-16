#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

//abstract class for generating an image from a camera
//TODO: implement direct illumination (Whitted) model, path tracer, volume raycaster, and bidirectional path tracer classes
#include "../scene.hpp"
#include "../image.hpp"
#include "../materials/mirror.hpp"
#include "../hit.hpp"

class RayTracer {
public:
    int maxDepth;
    //number of samples per pixel
    int sampleRate;

    bool jitter = false;

    Image takePicture(Scene & scene, int camIndex);
    Hit traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth);

    void findShade(Scene & scene, Hit & hit, int depth);
    Hit traceShadowRay(Scene & scene, ray & shadowRay, Hit & hit);
};

#endif