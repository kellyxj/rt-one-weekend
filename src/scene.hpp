//A scene consists of geometry objects, lights, and at least one camera
#include "geometry.hpp"
#include "plane.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "sphereLight.hpp"
#include <vector>

class Scene {
public:
    std::vector<Geometry*> items;
    std::vector<Camera*> cameras;
    std::vector<Geometry*> lights;
};