#ifndef __SPECTRALRENDERER_H__
#define __SPECTRALRENDERER_H__
#include "rayTracer.hpp"

class SpectralRenderer : public RayTracer{
public:
    int nChannels = Color().nSpectralSamples;
    Image takePicture(Scene & scene, int camIndex);
    // std::vector<std::vector<float>> createPdf();
};

#endif