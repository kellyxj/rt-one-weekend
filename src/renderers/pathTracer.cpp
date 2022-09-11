#include "pathTracer.hpp"

void PathTracer::findShade(Scene & scene, Hit & hit, int depth) {
    if(hit.t > 1e10) {
        hit.color = sky;
    }
    else if(hit.isLight) {
        hit.color = white;
    }
    else {
        
    }
}