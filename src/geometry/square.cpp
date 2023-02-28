#include "square.hpp"

Hit Square::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    float t0 = -origin.z/dir.z;
    vec4 modelSpacePos = origin + (dir * t0);

    Hit hit;
    if(t0 >= 0 && (modelSpacePos.x > -1 && modelSpacePos.x < 1 && modelSpacePos.y > -1 && modelSpacePos.y < 1)) {
        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = modelSpacePos;
        hit.normal = this->getNormal(hit.pos, ray);
        hit.material = this->material;
        hit.brightness = 0;
        hit.inRay = inRay;
    }
    return hit;
}
vec4 Square::getNormal(vec4 & pos, ray & inRay) {
    vec4 v(0,0,1,0);
    if(v.dot(inRay.direction) > 0) {
        v.z = -1;
    }
    vec4 normal = (this->normalToWorld).transform(v);
    normal.w = 0;
    normal.normalize();
    return normal;
}

json Square::serialize() {
    json json_ = {
        {"type", type},
        {"material", material->serialize()},
        {"transform", modelMatrix.serialize()}
    };

    return json_;
}

Geometry* Square::deserialize(json json_) {
    auto material_ = json_["material"];
    MaterialType type = (MaterialType)material_["type"];

    Square* square = new Square();

    switch(type) {
        case MaterialType::diffuse: {
            base* m = new base();
            m = dynamic_cast<base*>(m->deserialize(material_));
            square->setMaterial(*m);
            break;
        }
        case MaterialType::groundGrid: {
            groundGrid* m = new groundGrid();
            m = dynamic_cast<groundGrid*>(m->deserialize(material_));
            square->setMaterial(*m);
            break;
        }
        case MaterialType::dielectric: {
            Glass* m = new Glass();
            m = dynamic_cast<Glass*>(m->deserialize(material_));
            square->setMaterial(*m);
            break;
        }
        case MaterialType::conductor: {
            Mirror* m = new Mirror();
            m = dynamic_cast<Mirror*>(m->deserialize(material_));
            square->setMaterial(*m);
            break;
        }
        default:
            break;
    }
    auto modelMatrix_ = json_["transform"];
    mat4 modelMatrix;
    modelMatrix = modelMatrix.deserialize(modelMatrix_);
    square->modelMatrix = modelMatrix;

    square->worldToModel = modelMatrix.invert();
    square->normalToWorld = modelMatrix.transpose();

    return square;
}