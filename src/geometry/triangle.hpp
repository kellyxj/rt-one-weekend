#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "geometry.hpp"

//N.B.: modelMatrix of individual triangles is assumed to be identity. Use transforms on containing meshes instead
class Triangle : public Geometry {
public:
    GeometryType type = GeometryType::triangle;

    vec4 v0;
    vec4 v1;
    vec4 v2;
    Geometry* parent;

    Triangle();
    Triangle(vec4 & a, vec4 & b, vec4 & c);
    vec4 getNormal(vec4 & pos, ray & inRay);
    Hit trace(ray & inRay);

    json serialize();
};
#endif