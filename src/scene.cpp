#include "scene.hpp"

json Scene::serialize() {
    json json_ = {
        {"name", name},
        {"ambientLight", ambientLight},
        {"backgroundColor", {
            {"red", backgroundColor.r},
            {"green", backgroundColor.g},
            {"blue", backgroundColor.b}
        }},
        {"items", json::array()},
        {"cameras",json::array()}
    };
    for(Camera* camera : cameras) {
        json_["cameras"].push_back(camera->serialize());
    }
    for(Geometry* item: items) {
        json_["items"].push_back(item->serialize());
    }

    return json_;
}

Scene Scene::deserialize(json json_) {
    name = json_["name"];
    ambientLight = json_["ambientLight"];
    backgroundColor = backgroundColor.deserialize(json_["color"]);

    for(auto item : json_["items"]) {
        GeometryType type = item["type"];
        switch(type) {
            case GeometryType::plane:
                Plane* plane = new Plane();
                plane = dynamic_cast<Plane*>(plane->deserialize(item));
                items.push_back(plane);
                break;
            case GeometryType::sphere:
                Sphere* sphere = new Sphere();
                sphere = dynamic_cast<Sphere*>(sphere->deserialize(item));
                items.push_back(sphere);
                break;
            case GeometryType::square:
                Square* square = new Square();
                square = dynamic_cast<Square*>(square->deserialize(item));
                items.push_back(square);
                break;
            case GeometryType::mesh:
                Mesh* mesh = new Mesh();
                mesh = dynamic_cast<Mesh*>(mesh->deserialize(item));
                items.push_back(mesh);
                break;
            case GeometryType::triangle:
                Triangle* triangle = new Triangle();
                triangle = dynamic_cast<Triangle*>(triangle->deserialize(item));
                items.push_back(triangle);
                break;
            case GeometryType::bvh:
                BVH* bvh = new BVH();
                bvh = dynamic_cast<BVH*>(bvh->deserialize(item));
                items.push_back(bvh);
                break;
        }
    }
}