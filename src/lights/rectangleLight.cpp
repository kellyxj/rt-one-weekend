#include "rectangleLight.hpp"

RectangleLight::RectangleLight()
{
    width = 1;
    height = 1;
}
RectangleLight::RectangleLight(float w, float h)
{
    width = w;
    height = h;
}

float RectangleLight::area()
{
    // temporary: need to apply transforms
    return width * height;
}

vec4 RectangleLight::getPointOnLight()
{
    float x = 2 * width * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - .5);
    float y = 2 * height * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - .5);

    vec4 p(x, y, 0, 1);
    p = modelMatrix.transform(p);
    return p;
}
Hit RectangleLight::trace(ray &inRay)
{
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    float t0 = -origin.z / dir.z;
    vec4 modelSpacePos = origin + (dir * t0);

    Hit hit;
    if (t0 >= 0 && (modelSpacePos.x > -width / 2 && modelSpacePos.x < width / 2 && modelSpacePos.y > -height / 2 && modelSpacePos.y < height / 2))
    {
        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = modelSpacePos;
        hit.normal = this->getNormal(hit.pos, ray);
        hit.material = this->material;
        hit.brightness = this->material->brightness;
        hit.inRay = inRay;
    }
    return hit;
}
vec4 RectangleLight::getNormal(vec4 &pos, ray &inRay)
{
    vec4 v(0, 0, 1, 0);
    if (v.dot(inRay.direction) > 0)
    {
        v.z = -1;
    }
    vec4 normal = this->normalToWorld.transform(v);
    normal.w = 0;
    normal.normalize();
    return normal;
}

json RectangleLight::serialize()
{
    json json_ = {
        {"type", lightType},
        {"material", material->serialize()}};
    return json_;
}
Light *RectangleLight::deserialize(json json_)
{
}