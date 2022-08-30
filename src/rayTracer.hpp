//abstract class for generating an image from a camera
//TODO: implement direct illumination (Whitted) model, path tracer, volume raycaster, and bidirectional path tracer classes
#include "scene.hpp"
#include "image.hpp"
#include "hit.hpp"

class RayTracer {
public:
    Image takePicture(Scene & scene, int camIndex);
    Hit traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth);
};