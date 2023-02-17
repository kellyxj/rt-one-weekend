#ifndef __SCENE_H__

//A scene consists of geometry objects, lights, and at least one camera
#include "geometry/geometry.hpp"
#include "geometry/plane.hpp"
#include "camera.hpp"
#include "geometry/sphere.hpp"
#include "geometry/square.hpp"
#include "geometry/triangle.hpp"
#include <vector>

class Scene {
public:
    Color backgroundColor = sky;
    float ambientLight = .25;
    std::vector<Geometry*> items;
    std::vector<Camera*> cameras;
};
#endif