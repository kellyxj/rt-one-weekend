#include "rayTracer.hpp"
#include <iostream>

Image RayTracer::takePicture(Scene &scene, int camIndex)
{
    Camera *cam = (scene.cameras[camIndex]);
    Image output(cam->width, cam->height);
    for (int i = 0; i < output.width; i++)
    {
        for (int j = 0; j < output.height; j++)
        {
            Color c;
            output.pixels.push_back(c);
        }
    }

    int sampleRate = this->sampleRate;

    for (int j = (cam->height - 1); j >= 0; j--)
    {
        std::cout << j << "\n";
        for (int i = 0; i < (cam->width); i++)
        {
            for (int k = 0; k < sampleRate; k++)
            {
                float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);
                float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);

                randX -= .5;
                randY -= .5;

                ray eyeRay = cam->getEyeRay(i + .5 + randX, j + .5 + randY);

                Hit hit;
                hit = this->traceRay(scene, eyeRay, hit, 0);

                Color c = output.getPixel(i, j);
                c += hit.color / sampleRate * cam->exposure;

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
            for (auto entry : c.channels)
            {
                entry = (float)pow((double)entry, 1 / cam->gamma);
            }
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
    // closest.pdf = hit.pdf * eyeRay.pdf;
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
        base hemisphereSampler;

        float lightPdf;
        float hemispherePdf;

        // direct light contribution
        Hit shadowHit;
        ray shadowRay;
        // light importance sampling strategy
        for (Geometry *light : scene.lights)
        {
            Light *light_ = dynamic_cast<Light *>(light);

            vec4 lightVec;
            lightVec = light_->getPointOnLight() - hit.pos;
            float distance_squared = lightVec.length_squared();
            lightVec.normalize();
            shadowRay.origin = hit.pos + hit.normal * EPSILON;
            shadowRay.direction = lightVec;
            shadowHit = traceShadowRay(scene, shadowRay, shadowHit);

            if (shadowHit.brightness > 0)
            {
                float lambertian = lightVec.dot(hit.normal);
                lambertian = lambertian > 0 ? lambertian : 0;

                float brdf = hit.material->sampleBrdf(hit.inRay, shadowRay, hit.pos);

                float area = light_->area();
                float solidAngle = area * shadowHit.normal.dot(lightVec) * -1 / (distance_squared);
                lightPdf = solidAngle;

                hemispherePdf = hemisphereSampler.sampleBrdf(hit.inRay, shadowRay, hit.pos);
                // float powerHeuristic = (lightPdf * lightPdf)/(hemispherePdf * hemispherePdf + lightPdf * lightPdf);
                float balanceHeuristic = lightPdf / (hemispherePdf + lightPdf);

                Color radiance = (shadowHit.material->getColor(shadowHit.modelSpacePos)) * shadowHit.brightness;
                radiance *= solidAngle * lambertian;
                hit.color += radiance * brdf * balanceHeuristic;
            }
        }
        // hemisphere direct light sampling strategy
        shadowRay = hemisphereSampler.scatter(hit.inRay, hit.pos, hit.normal);
        shadowHit.brightness = 0;
        shadowHit = traceShadowRay(scene, shadowRay, shadowHit);

        if (shadowHit.brightness > 0)
        {
            vec4 lightVec = shadowHit.pos - hit.pos;
            float distance_squared = lightVec.length_squared();
            lightVec.normalize();

            float lambertian = lightVec.dot(hit.normal);
            lambertian = lambertian > 0 ? lambertian : 0;

            float brdf = hit.material->sampleBrdf(hit.inRay, shadowRay, hit.pos);

            hemispherePdf = hemisphereSampler.sampleBrdf(hit.inRay, shadowRay, hit.pos);
            Light *light = ((Light *)shadowHit.geometry);
            lightPdf = light->area() * shadowHit.normal.dot(lightVec) * -1 / (distance_squared);
            // float powerHeuristic = (hemispherePdf * hemispherePdf)/(hemispherePdf * hemispherePdf + lightPdf * lightPdf);
            float balanceHeuristic = hemispherePdf / (hemispherePdf + lightPdf);

            Color radiance = (shadowHit.material->getColor(shadowHit.modelSpacePos)) * shadowHit.brightness;
            radiance *= hitColor * lambertian;
            hit.color += radiance * brdf * balanceHeuristic;
        }

        Hit bounceHit;
        ray reflectedRay;
        // BRDF importance sampling
        reflectedRay = hit.material->scatter(hit.inRay, hit.pos, hit.normal);
        hit.color *= reflectedRay.pdf;
        if (depth < this->maxDepth)
        {
            bounceHit = this->traceRay(scene, reflectedRay, bounceHit, depth + 1);
        }
        // BRDF contribution
        if (bounceHit.brightness > 0)
        {
            hit.color += bounceHit.color * hitColor * bounceHit.brightness;
        }
        else
        {
            hit.color += hitColor * bounceHit.color;
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