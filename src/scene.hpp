//A scene consists of geometry objects, lights, and at least one camera
#include "geometry/geometry.hpp"
#include "geometry/plane.hpp"
#include "camera.hpp"
#include "geometry/sphere.hpp"
#include "geometry/sphereLight.hpp"
#include <vector>

class Scene {
public:
    std::vector<Geometry*> items;
    std::vector<Camera*> cameras;
    std::vector<Geometry*> lights;
};