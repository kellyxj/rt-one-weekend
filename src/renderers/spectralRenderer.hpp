#include "rayTracer.hpp"

class SpectralRenderer : public RayTracer{
public:
    int nChannels = 3;
    Image takePicture(Scene & scene, int camIndex);
};