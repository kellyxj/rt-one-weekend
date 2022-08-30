#include "geometry.hpp"
#include <iostream>

void Geometry::translate(vec4 & v) {
    setIdentity();
    modelMatrix = modelMatrix.translate(v);
    worldToModel = worldToModel.translate(v * -1);
    normalToWorld = worldToModel.transpose();
}

void Geometry::rotate(double angle, vec4 & axis){
    setIdentity();
    modelMatrix = modelMatrix.rotate(angle, axis);
    worldToModel = worldToModel.rotate(-angle, axis);
    normalToWorld = worldToModel.transpose();
}

void Geometry::scale(vec4 & v){
    setIdentity();
    modelMatrix = modelMatrix.scale(v);

    vec4 c(1/v.x, 1/v.y, 1/v.z, 0);
    worldToModel = worldToModel.scale(c);

    normalToWorld = worldToModel.transpose();
}