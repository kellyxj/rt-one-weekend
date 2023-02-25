#ifndef __MESH_H__
#define __MESH_H__

#include "triangle.hpp"
#include "bvh.hpp"
#include <vector>

class Mesh : public Geometry {
public:
    GeometryType type = GeometryType::mesh;
    std::string obj_name;

    std::vector<Geometry*> triangleList;
    BVH bvh = BVH();
    
    void setMaterial(Material & m);
    vec4 getNormal(vec4 & pos, ray & inRay);
    Hit trace(ray & inRay);

    void loadFromObj(std::string obj_name_);
    void constructBVH();

    json serialize();
    Geometry* deserialize(json json_);
};
#endif