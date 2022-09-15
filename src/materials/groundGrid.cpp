#include "groundGrid.hpp"

Color groundGrid::getColor(vec4 & pos) {
    double xPos = pos.x;
    double yPos = pos.y;
    double intPart;
    if(modf(xPos, &intPart) > .95 || modf(xPos, &intPart) < -.95 || modf(yPos, &intPart) > .95 || modf(yPos, &intPart) < -.95) {
        return green;
    }
    else if((modf(xPos, &intPart) >= 0 && modf(xPos, &intPart) < .05) || (modf(yPos, &intPart) >= 0 && modf(yPos, &intPart) < .05) || 
        (modf(xPos, &intPart) <= 0 && modf(xPos, &intPart) > -.05) || (modf(yPos, &intPart) <= 0 && modf(yPos, &intPart) > -.05)) 
    {
        return green;
    }
    else {
        return white;
    }
}

ray groundGrid::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    double theta = 2*PI*static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    double phi = 2*PI*static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    vec4 randomDirection;
    randomDirection.x += cos(phi) * sin(theta);
    randomDirection.y += sin(phi) * sin(theta);
    randomDirection.z += cos(theta);
    randomDirection.w = 0;
    if(randomDirection.dot(normal) < 0) {
        randomDirection *= -1;
    }
    ray outRay(pos, randomDirection);
    return outRay;
}