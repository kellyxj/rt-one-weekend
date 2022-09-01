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