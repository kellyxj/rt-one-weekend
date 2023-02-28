#include "sphere.hpp"

Hit Sphere::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    vec4 center(0,0,0,1);
    vec4 r2s = center-origin;

    float L2 = r2s.length_squared();
    float tcaS = dir.dot(r2s);

    Hit hit;
    //rays originating inside sphere
    if(L2 <= 1) {
        float DL2 = dir.length_squared();

        float tca2 = tcaS * tcaS/DL2;
        float LM2 = L2 - tca2;

        float L2hc = 1-LM2;
        float t0 = tcaS/DL2 + sqrt(L2hc/DL2);

        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = origin + (dir * t0);
        hit.normal = this->getNormal(hit.modelSpacePos, ray);
        hit.material = this->material;
        hit.brightness = 0;
        hit.inRay = inRay;
    }
    //if tcaS < 0, sphere is behind the camera
    else if(tcaS >= 0) {
        float DL2 = dir.length_squared();

        float tca2 = tcaS * tcaS/DL2;
        float LM2 = L2 - tca2;

        if(LM2 <= 1) {
            float L2hc = 1-LM2;
            float t0 = tcaS/DL2 - sqrt(L2hc/DL2);
            float t1 = tcaS/DL2 + sqrt(L2hc/DL2);

            hit.t = t0;
            hit.pos = inRay.origin + (inRay.direction * t0);
            hit.modelSpacePos = origin + (dir * t0);
            hit.normal = this->getNormal(hit.modelSpacePos, ray);
            hit.material = this->material;
            hit.brightness = 0;
            hit.inRay = inRay;
        }
    }
    return hit;
}

vec4 Sphere::getNormal(vec4 & pos, ray & inRay) {
    vec4 normVec(pos.x, pos.y, pos.z, 0);
    
    if(normVec.dot(inRay.direction) > 0) {
        normVec = normVec * -1;
    }
    normVec = (this->normalToWorld).transform(normVec);
    normVec.w = 0;
    return normVec.normalize();
}

json Sphere::serialize() {
    json json_ = {
        {"type", type},
        {"material", material->serialize()},
        {"transform", modelMatrix.serialize()}
    };

    return json_;
}

Geometry* Sphere::deserialize(json json_) {
    auto material_ = json_["material"];
    MaterialType type = (MaterialType)material_["type"];

    Sphere* sphere = new Sphere();

    switch(type) {
        case MaterialType::diffuse: {
            base* m = new base();
            m = dynamic_cast<base*>(m->deserialize(material_));
            sphere->setMaterial(*m);
            break;
        }
        case MaterialType::groundGrid: {
            groundGrid* m = new groundGrid();
            m = dynamic_cast<groundGrid*>(m->deserialize(material_));
            sphere->setMaterial(*m);
            break;
        }
        case MaterialType::dielectric: {
            Glass* m = new Glass();
            m = dynamic_cast<Glass*>(m->deserialize(material_));
            sphere->setMaterial(*m);
            break;
        }
        case MaterialType::conductor: {
            Mirror* m = new Mirror();
            m = dynamic_cast<Mirror*>(m->deserialize(material_));
            sphere->setMaterial(*m);
            break;
        }
        default:
            break;
    }
    auto modelMatrix_ = json_["transform"];
    mat4 modelMatrix;
    modelMatrix = modelMatrix.deserialize(modelMatrix_);
    sphere->modelMatrix = modelMatrix;

    sphere->worldToModel = modelMatrix.invert();
    sphere->normalToWorld = modelMatrix.transpose();

    return sphere;
}