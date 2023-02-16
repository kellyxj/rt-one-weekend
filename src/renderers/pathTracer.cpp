#include "pathTracer.hpp"

Image PathTracer::takePicture(Scene & scene, int camIndex) {
    Image output;
    Camera cam = (*scene.cameras[camIndex]);
    output.width = cam.width;
    output.height = cam.height;
    for(int i = 0; i < output.width; i++) {
        for(int j = 0; j < output.height; j++) {
            Color c;
            output.pixels.push_back(c);
        }
    }

    int sampleRate = this->sampleRate;

    for(int j = (cam.height-1); j >= 0; j--) {
        for(int i = 0; i < (cam.width); i++) {
            for(int k = 0; k < sampleRate; k++) {
                float randX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (sampleRate > 1 ? 1 : 0);
                float randY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (sampleRate > 1 ? 1 : 0);

                randX -= .5;
                randY -= .5;

                ray eyeRay = cam.getEyeRay(i+.5+randX, j+.5+randY);
                Hit hit;
                hit = this->traceRay(scene, eyeRay, hit, 0);

                Color c = output.getPixel(i,j);
                c.r += hit.color.r/sampleRate;
                c.g += hit.color.g/sampleRate;
                c.b += hit.color.b/sampleRate;

                output.setPixel(i,j,c);
            }
            
        }
    }
        
    return output;
}

Hit PathTracer::traceRay(Scene & scene, ray & eyeRay, Hit & hit, int depth) {
    Hit closest = hit;
    for(Geometry* item: scene.items) {
        if(item->getType() == 1) {
            Hit current = dynamic_cast<Plane*>(item)->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = false;
            }
        }
        else if(item->getType() == 2) {
            Hit current = dynamic_cast<Sphere*>(item)->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = false;
            }
        }
        else if(item->getType() == 3) {
            Hit current = dynamic_cast<Square*>(item)->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = false;
            }
         }
    }
    for(Geometry* light: scene.lights) {
        if(light->getType() == 2) {
            Hit current = light->trace(eyeRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = true;
            }
        }
    }
    this->findShade(scene, closest, depth);
    return closest;
}

void PathTracer::findShade(Scene & scene, Hit & hit, int depth) {
    if(hit.t > 1e10) {
        hit.color = sky;
    }
    else if(hit.isLight) {
        hit.color = white;
    }
    else {
        ray reflectedRay;

        //vec4 translatedHitPos = hit.pos+hit.normal*EPSILON;

        std::cout << hit.material;
        reflectedRay = hit.material->scatter(hit.inRay, hit.pos, hit.normal);

        Hit reflectedHit;
        if(depth < this->maxDepth) {
            this->traceRay(scene, reflectedRay, reflectedHit, depth+1);
        }
        if(reflectedHit.isLight) {
            hit.color.r += .85 * hit.material->getColor(hit.modelSpacePos).r;
            hit.color.g += .85 * hit.material->getColor(hit.modelSpacePos).g;
            hit.color.b += .85 * hit.material->getColor(hit.modelSpacePos).b;
        }
        hit.color.r += .15 * hit.material->getColor(hit.modelSpacePos).r;
        hit.color.g += .15 * hit.material->getColor(hit.modelSpacePos).g;
        hit.color.b += .15 * hit.material->getColor(hit.modelSpacePos).b;
    }
    
}