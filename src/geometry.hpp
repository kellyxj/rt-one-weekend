#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "mat4.hpp"
#include "material.hpp"
#include "hit.hpp"
#include "vec4.hpp"

class Geometry {
public:
    mat4 modelMatrix;
    mat4 worldToModel;
    mat4 normalToWorld;

    Material m;

    //void setMaterial(Material & m);
    vec4 getNormal(vec4 & pos);

    void setIdentity() {
        modelMatrix.setIdentity();
        worldToModel.setIdentity();
        normalToWorld.setIdentity();
    }

    //void translate(vec4 & v);
    //void rotate(double angle, vec4 & axis);
    //void scale(vec4 & v);

    Hit trace(ray & inRay);
};

#endif