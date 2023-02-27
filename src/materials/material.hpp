#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../defines.hpp"
#include "../math/vec4.hpp"
#include "../color.hpp"
#include "../math/ray.hpp"
#include <cmath>

enum class MaterialType
{
    none,
    diffuse,
    groundGrid,
    dielectric,
    conductor
};

class Material
{
public:
    MaterialType type = MaterialType::none;
    float brightness = 0;
    bool isDelta = false;

    virtual Color getColor(vec4 &pos) = 0;
    virtual ray scatter(ray &inRay, vec4 &pos, vec4 &normal) = 0;

    virtual json serialize() = 0;
    virtual Material *deserialize(json json_) = 0;

    virtual float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos) = 0;

    virtual ~Material() {}
};

#endif