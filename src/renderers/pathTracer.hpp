#include "rayTracer.hpp"

class PathTracer: public RayTracer {
public:
    Image takePicture(Scene & scene, int camIndex);
    Hit traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth);
    void findShade(Scene & scene, Hit & hit, int depth);
};