#include "groundCheckerboard.hpp"

Color groundCheckerboard::getColor(vec4 &pos)
{
    float xPos = pos.x;
    float yPos = pos.y;
    // checkborad pattern
    if (fmod(floor(xPos) + floor(yPos), 2.0) == 0.0)
    {
        return black;
    }
    else
    {
        return white;
    }
}

float groundCheckerboard::sampleBrdf(ray &inRay, ray& outRay, vec4 &pos) {
    return 1/(2*PI);
}

ray groundCheckerboard::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    float theta = 2*PI*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float phi = 2*PI*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    vec4 randomDirection;
    randomDirection += normal.normalize();
    randomDirection.x += cos(phi) * sin(theta);
    randomDirection.y += sin(phi) * sin(theta);
    randomDirection.z += cos(theta);
    randomDirection.w = 0;
    
    ray outRay(pos+normal*EPSILON, randomDirection);
    return outRay;
}

json groundCheckerboard::serialize() {
    json json_ = {
        {"type", type}
    };
    return json_;
}