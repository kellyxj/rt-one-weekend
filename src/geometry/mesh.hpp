#ifndef __MESH_H__

#include "triangle.hpp"
#include <vector>

class Mesh : public Geometry {
public:
    std::vector<Geometry*> triangleList;

    void setMaterial(Material & m);
    vec4 getNormal(vec4 & pos, ray & inRay);
    Hit trace(ray & inRay);

    void loadFromObj(std::string filepath);
};
#endif