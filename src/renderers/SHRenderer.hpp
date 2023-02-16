#ifndef __SHRENDERER_H__
#define __SHRENDERER_H__

#include "rayTracer.hpp"

class SHRenderer : RayTracer {
public:
    void findShade(Scene & scene, Hit & hit, int depth);
};

#endif