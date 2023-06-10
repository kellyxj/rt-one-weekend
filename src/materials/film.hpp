#ifndef __FILM_H__
#define __FILM_H__

#include "material.hpp"

class Film : public Material
{
public:
    MaterialType type = MaterialType::dielectric;
    bool isDelta = true;
    float thickness = 600;   // 0~3000 250 #Thin film thickness(in nm)
    float externalIOR = 1;   // 0.2~3 1 #External(air) refractive index
    float thinfilmIOR = 1.4; // 0.2~3 1.5 #Layer(thin film) refractive index
    float internalIOR = 1;   // 0.2~3 1.25 #Internal(object) refractive index
    // float n = 100;           // 1~1000 100 #Blinn - Phong microfacet exponent
    IOR n_i;
    Color c;
    Color getColor(vec4 &pos);
    ray scatter(ray &inRay, vec4 &pos, vec4 &normal);
    float sampleBrdf(ray &inRay, ray &outRay, vec4 &pos);

    float *fresnelCoating(float cos0, ray &inRay, ray &outRay);
    json serialize();
};

#endif