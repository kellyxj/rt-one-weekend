#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene &scene, int camIndex)
{
    Image output;
    Camera cam = (*scene.cameras[camIndex]);
    output.width = cam.width;
    output.height = cam.height;
    for (int i = 0; i < output.width; i++)
    {
        for (int j = 0; j < output.height; j++)
        {
            Color c;
            output.pixels.push_back(c);
        }
    }

    int sampleRate = this->sampleRate;

    for (int j = (cam.height - 1); j >= 0; j--)
    {
        std::cout << j << "\n";
        for (int i = 0; i < (cam.width); i++)
        {
            for (int k = 0; k < sampleRate; k++)
            {
                float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);
                float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);

                randX -= .5;
                randY -= .5;

                ray eyeRay = cam.getEyeRay(i + .5 + randX, j + .5 + randY);
                Hit hit;
                hit = this->traceRay(scene, eyeRay, hit, 0);

                Color c = output.getPixel(i, j);
                c.r += hit.color.r / sampleRate * cam.exposure;
                c.g += hit.color.g / sampleRate * cam.exposure;
                c.b += hit.color.b / sampleRate * cam.exposure;

                // normal map
                // c.r += (hit.normal.x + 1)/(2*sampleRate);
                // c.g += (hit.normal.y + 1)/(2*sampleRate);
                // c.b += (hit.normal.z + 1)/(2*sampleRate);

                // unnormalized depth map
                // c.r += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;
                // c.g += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;
                // c.b += sqrt((hit.pos-cam.eyePoint).length())/sampleRate;

                output.setPixel(i, j, c);
            }
            Color c = output.getPixel(i, j);
            c.r = (float)pow((double)c.r, 1 / cam.gamma);
            c.g = (float)pow((double)c.g, 1 / cam.gamma);
            c.b = (float)pow((double)c.b, 1 / cam.gamma);
            output.setPixel(i, j, c);
        }
    }

    return output;
}

Hit RayTracer::traceRay(Scene &scene, ray &eyeRay, Hit &hit, int depth)
{
    Hit closest = hit;
    for (Geometry *item : scene.items)
    {
        Hit current = item->trace(eyeRay);
        if (current.t < closest.t)
        {
            closest = current;
        }
    }
    for (Geometry *light : scene.lights)
    {
        Hit current = light->trace(eyeRay);
        if (current.t < closest.t)
        {
            closest = current;
        }
    }
    this->findShade(scene, closest, depth);
    return closest;
}

void RayTracer::findShade(Scene &scene, Hit &hit, int depth)
{
    // missed
    if (hit.t > 1e10)
    {
        hit.color = scene.backgroundColor;
        hit.brightness = scene.ambientLight;
    }
    // hit
    else
    {
        // Phong shading
        /*
        for(Geometry* light: scene.lights) {
            vec4 lightCenter(0,0,0,1);
            lightCenter = light->modelMatrix.transform(lightCenter);

            ray shadowRay(hit.pos+lightCenter*EPSILON, lightCenter-hit.pos);
            Hit shadowHit;
            shadowHit = this->traceShadowRay(scene, shadowRay, shadowHit);

            if(shadowHit.isLight) {
                Color hitColor = hit.material->getColor(hit.modelSpacePos);
                vec4 N = (hit.normal).normalize();
                vec4 L = (lightCenter - hit.pos).normalize();
                float lambertian = N.dot(L);
                lambertian = lambertian < 0 ? 0 : lambertian;

                hit.color.r += hitColor.r * lambertian * light->brightness;
                hit.color.g += hitColor.g * lambertian * light->brightness;
                hit.color.b += hitColor.b * lambertian * light->brightness;
            }

        }

        hit.color.r += .15;
        hit.color.b += .15;
        hit.color.g += .15;
        */
        Color hitColor = hit.material->getColor(hit.modelSpacePos);

        // direct light contribution
        if(mode == RenderMode::direct) {
            Hit shadowHit;
            ray shadowRay;

            for (Geometry *light : scene.lights)
            {
                Light *light_ = dynamic_cast<Light *>(light);

                vec4 lightVec;
                lightVec = light_->getPointOnLight() - hit.pos;
                float distance = lightVec.length();
                lightVec.normalize();
                shadowRay.origin = hit.pos + hit.normal * EPSILON;
                shadowRay.direction = lightVec;
                shadowHit = traceShadowRay(scene, shadowRay, shadowHit);

                if (shadowHit.brightness > 0)
                {
                    float area = light_->area();
                    float solidAngle = area * shadowHit.normal.dot(lightVec) * -1/(distance * distance);

                    float radiance_r = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).r;
                    radiance_r *= solidAngle;
                    hit.color.r += radiance_r;

                    float radiance_g = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).g;
                    radiance_g *= solidAngle;
                    hit.color.g += radiance_g;

                    float radiance_b = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).b;
                    radiance_b *= solidAngle;
                    hit.color.b += radiance_b;
                }
            }
        }
        else if(mode == RenderMode::hemisphere) {
            Hit shadowHit;
            ray shadowRay;

            base hemisphereSampler;
            shadowRay = hemisphereSampler.scatter(hit.inRay, hit.pos, hit.normal);
            shadowHit = traceShadowRay(scene, shadowRay, shadowHit);
            
            if(shadowHit.brightness > 0) {
                vec4 lightVec = shadowHit.pos - hit.pos;
                float distance = lightVec.length();
                lightVec.normalize();

                float lambertian = lightVec.dot(hit.normal);
                lambertian = lambertian > 0 ? lambertian : 0;

                float radiance_r = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).r;
                radiance_r *= hitColor.r * lambertian;
                hit.color.r += radiance_r;

                float radiance_g = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).g;
                radiance_g *= hitColor.g * lambertian;
                hit.color.g += radiance_g;

                float radiance_b = shadowHit.brightness * (shadowHit.material->getColor(shadowHit.modelSpacePos)).b;
                radiance_b *= hitColor.b * lambertian;
                hit.color.b += radiance_b;
            }
        }

        Hit bounceHit;
        ray reflectedRay;

        reflectedRay = hit.material->scatter(hit.inRay, hit.pos, hit.normal);

        if (depth < this->maxDepth)
        {
            bounceHit = this->traceRay(scene, reflectedRay, bounceHit, depth + 1);
        }
        // BRDF contribution
        if (bounceHit.brightness > 0)
        {
            hit.color.r += (bounceHit.brightness) * bounceHit.color.r * hitColor.r;
            hit.color.g += (bounceHit.brightness) * bounceHit.color.g * hitColor.g;
            hit.color.b += (bounceHit.brightness) * bounceHit.color.b * hitColor.b;
        }
        else
        {
            hit.color.r += hitColor.r * bounceHit.color.r;
            hit.color.g += hitColor.g * bounceHit.color.g;
            hit.color.b += hitColor.b * bounceHit.color.b;
        }

        // light sources don't scatter (blackbody assumption)
        if (hit.brightness > 0)
        {
            hit.color = hitColor;
        }
    }
}

Hit RayTracer::traceShadowRay(Scene &scene, ray &shadowRay, Hit &hit)
{
    Hit closest = hit;

    for (Geometry *item : scene.items)
    {
        Hit current = item->trace(shadowRay);
        if (current.t < closest.t)
        {
            closest = current;
        }
    }
    for (Geometry *light : scene.lights)
    {
        Hit current = light->trace(shadowRay);
        if (current.t < closest.t)
        {
            closest = current;
        }
    }
    return closest;
}