#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene & scene, int camIndex) {
    Image output;
    Camera cam = (*scene.cameras[camIndex]);
    output.width = cam.width;
    output.height = cam.height;
    for(int i = 0; i < output.width; i++) {
        for(int j = 0; j < output.height; j++) {
            color c;
            output.pixels.push_back(c);
        }
    }

    int sampleRate = this->sampleRate;

    for(int j = (cam.height-1); j >= 0; j--) {
        for(int i = 0; i < (cam.width); i++) {
            for(int k = 0; k < sampleRate; k++) {
                double randX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                double randY = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

                randX -= .5;
                randY -= .5;

                ray eyeRay = cam.getEyeRay(i+.5+randX, j+.5+randY);
                Hit hit;
                hit = this->traceRay(scene, eyeRay, hit, 0);
                if(hit.t < 1e10) {
                    color c = output.getPixel(i,j);
                    color hitColor = hit.material->getColor(hit.modelSpacePos);

                    c.r += hitColor.r/sampleRate;
                    c.g += hitColor.g/sampleRate;
                    c.b += hitColor.b/sampleRate;
                    output.setPixel(i,j,c);
                }
                else {
                    color c = output.getPixel(i,j);
                    color blue(0,0,1);
                    c.r += blue.r/sampleRate;
                    c.g += blue.g/sampleRate;
                    c.b += blue.b/sampleRate;
                    output.setPixel(i,j,c);
                }
            }
            
        }
    }
    return output;
}

Hit RayTracer::traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth) {
    Hit closest = hit;
    for(Geometry* item: scene.items) {
        if(item->getType() == 1) {
            Hit current = dynamic_cast<Plane*>(item)->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
            }
        }
        /*else if(item->getType() == 2) {
            Hit current = dynamic_cast<Sphere*>(item)->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
            }
        }*/
        
    }
    return closest;
}