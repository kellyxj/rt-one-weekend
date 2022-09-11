#include "rayTracer.hpp"

class PathTracer: public RayTracer {
public:
    void findShade(Scene & scene, Hit & hit, int depth);
};