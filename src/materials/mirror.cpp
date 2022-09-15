#include "mirror.hpp"

Color Mirror::getColor(vec4 & pos) {
    return this->c;
}

ray Mirror::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    vec4 reflectedDir;
    ray outRay(pos, reflectedDir);
    return outRay;
}