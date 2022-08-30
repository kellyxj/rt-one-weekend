//A scene consists of geometry objects, lights, and at least one camera
#include "geometry.hpp"
#include "plane.hpp"
#include "camera.hpp"
#include <vector>

class Scene {
public:
    std::vector<Geometry*> items;
    std::vector<Camera*> cameras;

    int maxDepth;
    int sampleRate;

};