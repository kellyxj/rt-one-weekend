#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

//abstract class for generating an image from a camera
#include "../scene.hpp"
#include "../image.hpp"
#include "../materials/mirror.hpp"
#include "../materials/glass.hpp"
#include "../hit.hpp"

enum class RenderMode {direct, hemisphere, brdf};

class RayTracer {
public:
    int maxDepth;
    //number of samples per pixel
    int sampleRate;

    bool jitter = false;

    Image takePicture(Scene & scene, int camIndex, float t);
    std::vector<Image*> takeVideo(Scene & scene, int camIndex, float start, float duration, int frameRate);
    Hit traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth, float t);

    void findShade(Scene & scene, Hit & hit, int depth, float t);
    Hit traceShadowRay(Scene & scene, ray & shadowRay, Hit & hit, float t);
};

#endif