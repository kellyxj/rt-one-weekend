#include "colorChange.hpp"

Color ColorChange::getColor(vec4 &pos, float t)
{

    float tFrac = t/2 - ((int) t/2);

    float r;
    float g;
    float b;

    if (tFrac <= 1.0/6) {
        r = 1;
        g = tFrac * 6;
        b = 0;
    }
    else if (tFrac <= 1.0/3) {
        r = 1 - (tFrac - 1.0/6) * 6;
        g = 1;
        b = 0;
    }
    else if (tFrac <= 1.0/2) {
        r = 0;
        g = 1;
        b = (tFrac - 1.0/3) * 6;
    }
    else if (tFrac <= 2.0/3) {
        r = 0;
        g = 1 - (tFrac - 1.0/2) * 6;
        b = 1;
    }
    else if (tFrac <= 5.0/6) {
        r = (tFrac - 2.0/3) * 6;
        g = 0;
        b = 1;
    }
    else {
        r = 1;
        g = 0;
        b = 1 - (tFrac - 5.0/6) * 6;
    }

    Color col = Color(r, g, b);
    return col;
}

ray ColorChange::scatter(ray &inRay, vec4 &pos, vec4 &normal)
{
    float theta = 2 * PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float phi = 2 * PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    vec4 randomDirection;
    randomDirection += normal.normalize();
    randomDirection.x += cos(phi) * sin(theta);
    randomDirection.y += sin(phi) * sin(theta);
    randomDirection.z += cos(theta);
    randomDirection.w = 0;

    ray outRay(pos + normal * EPSILON, randomDirection);
    return outRay;
}

float ColorChange::sampleBrdf(ray &inRay, ray& outRay, vec4 &pos) {
    return 1/(2*PI);
}

json ColorChange::serialize()
{
    json json_ = {
        {"type", type},
        {"brightness", brightness},
        {"color", c.serialize()}};
    return json_;
}