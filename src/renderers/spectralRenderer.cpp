#include <vector>
#include "spectralRenderer.hpp"

// Binomial distribution
// std::vector<std::vector<float>> SpectralRenderer::createPdf()
// {
//     std::vector<std::vector<std::vector<float>>> ret;
//     std::vector<std::vector<float>> ret2;
//     std::vector<float> ret3;
//     ret.reserve(nChannels);
//     ret2.reserve(nChannels);
//     ret3.reserve(nChannels);
//     for (int i = 0; i < nChannels; i++)
//         ret3.push_back(0.0f);
//     for (int i = 0; i < nChannels; i++)
//         ret2.push_back(ret3);
//     for (int i = 0; i < nChannels; i++)
//         ret.push_back(ret2);
//     for (int k = 0; k < nChannels; k++)
//     {
//         float p = static_cast<float>(k) / (nChannels - 1);
//         ret[0][0][k] = 1.0;
//         for (int i = 1; i < nChannels; i++)
//             ret[i][0][k] = (1.0 - p) * ret[i - 1][0][k];

//         for (int j = 1; j < nChannels; j++)
//             ret[0][j][k] = 0.0;

//         for (int i = 1; i < nChannels; i++)
//             for (int j = 1; j < nChannels; j++)
//                 ret[i][j][k] = (1.0 - p) * ret[i - 1][j][k] + p * ret[i - 1][j - 1][k];
//     }

//     return ret[nChannels - 1];
// };

Image SpectralRenderer::takePicture(Scene &scene, int camIndex)
{
    // std::vector<std::vector<float>> pdf = createPdf();

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
            Color c = output.getPixel(i, j);
            for (int k = 0; k < sampleRate; k++)
            {

                float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);
                float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (sampleRate > 1 ? 1 : 0);

                randX -= .5;
                randY -= .5;

                for (int p = 0; p < nChannels; p++)
                {
                    // int p = rand() % nChannels;
                    ray eyeRay = cam->getEyeRay(i + .5 + randX, j + .5 + randY);
                    eyeRay.color.channels[p] = 1;
                    // Try to use Binomial distribution to reduce spectral noise (no observable difference)
                    // for (int l = 0; l < nChannels; l++)
                    // {
                    //     eyeRay.color.channels[l] = pdf[l][p];
                    // }
                    // eyeRay.color.channels[n] = 1;

                    Hit hit;
                    hit = this->traceRay(scene, eyeRay, hit, 0);

                    c.channels[p] += hit.color.channels[p] / nChannels; // use Normal distribution to reduce spectral noise
                }
            }
            c *= cam->exposure / sampleRate;
            output.setPixel(i, j, c);
        }
    }
    // for (int j = (cam->height - 1); j >= 0; j--)
    // {
    //     for (int i = 0; i < cam->width; i++)
    //     {
    //         Color c = output.getPixel(i, j);
    //         for (auto entry : c.channels)
    //         {
    //             entry = (float)pow((double)entry, 1 / cam->gamma);
    //         }
    //         output.setPixel(i, j, c);
    //     }
    // }

    return output;
}