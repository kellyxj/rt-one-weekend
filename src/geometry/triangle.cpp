#include "triangle.hpp"
Triangle::Triangle() {
    v0 = vec4();
    v1 = vec4();
    v2 = vec4();
}

Triangle::Triangle(vec4 & a, vec4 & b, vec4 & c) {
    v0 = a;
    v1 = b;
    v2 = c;
}

vec4 Triangle::getNormal(vec4 & pos, ray & inRay) {
    vec4 edge1 = v1-v0;
    vec4 edge2 = v2-v0;
    vec4 normal = edge1.cross(edge2);
    if(normal.dot(inRay.direction) > 0) {
        normal = normal * -1;
    }
    normal = (parent->worldToModel).transform(normal);
    normal.normalize();
    return normal;
}

//Möller–Trumbore intersection algorithm (https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm)
Hit Triangle::trace(ray & inRay) {
    vec4 origin;
    vec4 dir;

    origin = inRay.origin;
    dir = inRay.direction;

    ray ray(origin, dir);

    vec4 edge1 = v1-v0;
    vec4 edge2 = v2-v0;

    //project to plane
    vec4 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);
    Hit hit;
    //parallel component to plane is 0
    if(a < EPSILON && a > -EPSILON) {
        return hit;
    }
    float f= 1.0/a;
    vec4 s = ray.origin - v0;
    //convert to barycentric
    float u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
    {
        return hit;
    }

    vec4 q = s.cross(edge1);
    float v = f * inRay.direction.dot(q);
    if (v < 0.0 || u+v > 1.0)
    {
        return hit;
    }

    float t = f * edge2.dot(q);

    //t < 0 means triangle behind origin
    if(t > 0) {
        
        hit.t = t;
        hit.pos = inRay.origin + (inRay.direction * t);
        hit.modelSpacePos = ray.origin + (ray.direction * t);
        hit.normal = getNormal(hit.modelSpacePos, ray);
        hit.material = this->material;
        hit.brightness = hit.material->brightness;
        hit.inRay = inRay;
    }
    return hit;
}