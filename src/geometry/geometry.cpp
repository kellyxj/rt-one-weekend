#include "geometry.hpp"
#include <iostream>

void Geometry::setMaterial(Material & m) {
    this->material = &m;
}

//Note: first translate an object into position, then scale it to the desired size
//doing it in the other order means the translation also gets scaled
void Geometry::translate(vec4 & v) {
    modelMatrix = modelMatrix.translate(v);
    worldToModel = worldToModel.translate(v * -1);
    normalToWorld = worldToModel.transpose();
}

void Geometry::rotate(float angle, vec4 & axis){
    modelMatrix = modelMatrix.rotate(angle, axis);
    worldToModel = worldToModel.rotate(-angle, axis);
    normalToWorld = worldToModel.transpose();
}

void Geometry::scale(vec4 & v){
    modelMatrix = modelMatrix.scale(v);

    vec4 c(1/v.x, 1/v.y, 1/v.z, 0);
    mat4 id;
    id = id.scale(c);
    worldToModel = id.multiply(worldToModel);

    normalToWorld = worldToModel.transpose();
}