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
    Hit closest;

    closest = bvh.trace(inRay);
    /*
    for(Geometry * triangle : triangleList) {
        Hit hit;
        hit = triangle->trace(inRay);
        if(hit.t < closest.t ) {
            closest = hit;
        }
    }*/

    return closest;
}

void Mesh::loadFromObj(std::string obj_name_) {
    obj_name = obj_name_;
    std::string inputfile = "../assets/" + obj_name;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ""; // Path to material files
    //N.B.: reader_config.triangulate is true by default

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

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            vec4 vertices[3];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vertices[v].x = attrib.vertices[3*size_t(idx.vertex_index)+0];
                vertices[v].y = attrib.vertices[3*size_t(idx.vertex_index)+1];
                vertices[v].z = attrib.vertices[3*size_t(idx.vertex_index)+2];
                vertices[v].w = 1;

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                }

                // Optional: vertex colors
                //tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                //tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                //tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }
            index_offset += fv;
            Triangle* triangle = new Triangle(vertices[0], vertices[1], vertices[2]);

            // per-face material
            if(materials.size() > 0) {
                tinyobj::material_t mat = materials[shapes[s].mesh.material_ids[f]];  
                base* triangleMat = new base();
                //diffuse only for now
                triangleMat->c.r = mat.diffuse[0];
                triangleMat->c.g = mat.diffuse[1];
                triangleMat->c.b = mat.diffuse[2];
                triangle->setMaterial(*triangleMat);
            }
            
            triangle->parent = this;

            triangleList.push_back(triangle);
        }
    }
}

void Mesh::constructBVH() {

    for(Geometry* triangle : triangleList) {
        Triangle* triangle_ = dynamic_cast<Triangle*>(triangle);

        //the initial state of the bvh has a leaf node bounding box around each triangle
        //these will ultimately be deleted during the call to bvh.build()
        BVH* leafNode = new BVH();
        leafNode->include(triangle_->v0);
        leafNode->include(triangle_->v1);
        leafNode->include(triangle_->v2);
        *leafNode = leafNode->transform(modelMatrix);
        //leafNode->children.push_back(triangle);
        
        bvh.coalesce(*leafNode);
        //bvh.children.push_back((Geometry*)leafNode);
        bvh.children.push_back(triangle);
    }
    bvh = bvh.build(13);
}

json Mesh::serialize() {
    json json_ = {
        {"type", type},
        {"transform", modelMatrix.serialize()},
        {"obj_name", obj_name}
    };

    return json_;
}
Geometry* Mesh::deserialize(json json_) {

}