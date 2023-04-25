#include "spectralRenderer.hpp"

Image SpectralRenderer::takePicture(Scene &scene, int camIndex)
{
    Camera* cam = (scene.cameras[camIndex]);
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

    for(int n = 0; n < nChannels; n++) {
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
                    eyeRay.color.channels[n] += 1;

                    Hit hit;
                    hit = this->traceRay(scene, eyeRay, hit, 0);

                    Color c = output.getPixel(i, j);
                    c += hit.color * eyeRay.color / sampleRate * cam->exposure;

                    output.setPixel(i, j, c);
                }
            }
        }
    }
    for(int j = (cam->height - 1); j >= 0; j--) {
        for(int i = 0; i < cam->width; i++) {
            Color c = output.getPixel(i, j);
            for(auto entry : c.channels) {
                entry = (float)pow((double)entry, 1/cam->gamma);
            }
            output.setPixel(i, j, c);
        }
    }

    return output;
}