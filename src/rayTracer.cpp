#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene & scene, int camIndex) {
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
                double randX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * (sampleRate > 1 ? 1 : 0);
                double randY = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * (sampleRate > 1 ? 1 : 0);

                randX -= .5;
                randY -= .5;

                ray eyeRay = cam.getEyeRay(i+.5+randX, j+.5+randY);
                Hit hit;
                hit = this->traceRay(scene, eyeRay, hit, 0);

                Color c = output.getPixel(i,j);
                c.r += hit.color.r/sampleRate;
                c.g += hit.color.g/sampleRate;
                c.b += hit.color.b/sampleRate;

                //normal map
                //c.r += (hit.normal.x + 1)/(2*sampleRate);
                //c.g += (hit.normal.y + 1)/(2*sampleRate);
                //c.b += (hit.normal.z + 1)/(2*sampleRate);

                //unnormalized depth map
                //c.r += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;
                //c.g += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;
                //c.b += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;

                output.setPixel(i,j,c);
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
    if(!closest.isLight) {
        this->findShade(scene, closest, depth);
    }
    else {
        closest.color = white;
    }
    return closest;
}

void RayTracer::findShade(Scene & scene, Hit & hit, int depth) {
    //missed
    if(hit.t > 1e10) {
        hit.color = blue;
    }
    //hit
    else {
        for(Geometry* light: scene.lights) {
            vec4 lightCenter(0,0,0,1);
            lightCenter = light->modelMatrix.transform(lightCenter);

            ray shadowRay(hit.pos+lightCenter*EPSILON, lightCenter-hit.pos, 0);
            Hit shadowHit;
            shadowHit = this->traceShadowRay(scene, shadowRay, shadowHit);

            if(shadowHit.isLight) {
                Color hitColor = hit.material->getColor(hit.modelSpacePos);
                vec4 N = (hit.normal).normalize();
                vec4 L = (lightCenter - hit.pos).normalize();
                double lambertian = N.dot(L);
                lambertian = lambertian < 0 ? 0 : lambertian;

                hit.color.r += hitColor.r * lambertian * light->brightness;
                hit.color.g += hitColor.g * lambertian * light->brightness;
                hit.color.b += hitColor.b * lambertian * light->brightness;
            }
            hit.color.r += .15;
            hit.color.b += .15;
            hit.color.g += .15;
        }
    }
}

Hit RayTracer::traceShadowRay(Scene & scene, ray & shadowRay, Hit & hit) {
    Hit closest = hit;

    for(Geometry* item: scene.items) {
        if(item->getType() == 1) {
            Hit current = dynamic_cast<Plane*>(item)->trace(shadowRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = false;
            }
        }
        else if(item->getType() == 2) {
            Hit current = dynamic_cast<Sphere*>(item)->trace(shadowRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = false;
            }
        }
    }
    for(Geometry* light: scene.lights) {
        if(light->getType() == 2) {
            Hit current = light->trace(shadowRay);
            if(current.t < closest.t) {
                closest = current;
                closest.isLight = true;
            }
        }
    }
    return closest;
}