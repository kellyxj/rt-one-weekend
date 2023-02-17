#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../util/tiny_obj_loader.h"

void Mesh::setMaterial(Material& m) {
    for(Geometry* triangle : triangleList) {
        triangle->setMaterial(m);
    }
}
//this should never be called
vec4 Mesh::getNormal(vec4 & pos, ray & inRay) {
    vec4 normal;
    return normal;
}

Hit Mesh::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    Hit closest;

    for(Geometry* triangle : triangleList) {
        Hit hit = triangle->trace(ray);
        if(hit.t < closest.t) {
            closest = hit;
        }
    }

    return closest;
}

void Mesh::loadFromObj(std::string filepath) {
    std::string inputfile = filepath;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./data"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> vertices;
    std::vector<float> normals;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];  

        }
    }
    int num_verts = vertices.size();
    for(int i = 0; i < num_verts; i+=9) {
        vec4 v0(vertices[i], vertices[i+1], vertices[i+2]);
        v0 /= 230;
        vec4 v1(vertices[i+3], vertices[i+4], vertices[i+5]);
        v1 /= 230;
        vec4 v2(vertices[i+6], vertices[i+7], vertices[i+8]);
        v2 /= 230;

        v0.w = 1;
        v1.w = 1;
        v2.w = 1;
        
        Triangle* triangle = new Triangle(v0, v1, v2);
        triangleList.push_back(triangle);
    }
}