#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene & scene, int camIndex) {
    Image output;
    Camera cam = (*scene.cameras[camIndex]);
    output.width = cam.width;
    output.height = cam.height;

    for(int j = cam.height-1; j >= 0; j--) {
        for(int i = 0; i < cam.width; i++) {
            ray eyeRay = cam.getEyeRay(i+.5, j+.5);
            Hit hit;
            hit = this->traceRay(scene, eyeRay, hit, 0);
            if(hit.t < 1e10) {
                double xPos = hit.pos.x;
                double yPos = hit.pos.y;
                double intPart;
                if(modf(xPos, &intPart) > .9 || modf(xPos, &intPart) < -.9 || modf(yPos, &intPart) > .9 || modf(yPos, &intPart) < -.9) {
                    color c(0, 255, 0);
                    output.pixels.push_back(c);
                }
                else if((modf(xPos, &intPart) > 0 && modf(xPos, &intPart) < .1) || (modf(yPos, &intPart) > 0 && modf(yPos, &intPart) < .1) || 
                        (modf(xPos, &intPart) < 0 && modf(xPos, &intPart) > -.1) || (modf(yPos, &intPart) < 0 && modf(yPos, &intPart) > -.1)) 
                {
                    color c(0, 255, 0);
                    output.pixels.push_back(c);
                }
                else {
                    color c(255, 255, 255);
                    output.pixels.push_back(c);
                }  
            }
            else {
                color c(0, 0, 255);
                output.pixels.push_back(c);
            }
        }
    }
    return output;
}

Hit RayTracer::traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth) {
    Hit closest = hit;
    for(Geometry* item: scene.items) {
        Hit current = dynamic_cast<Plane*>(item)->trace(eyeRay);
        if(current.t < closest.t) {
            closest = current;
        }
    }
    return closest;
}