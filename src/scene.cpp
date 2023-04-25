#include "scene.hpp"

json Scene::serialize() {
    json json_ = {
        {"name", name},
        {"ambientLight", ambientLight},
        {"backgroundColor", backgroundColor.serialize()},
        {"items", json::array()},
        {"cameras",json::array()}
    };
    for(Camera* camera : cameras) {
        json_["cameras"].push_back(camera->serialize());
    }
    for(Geometry* item: items) {
        json_["items"].push_back(item->serialize());
    }
    for(Light* light: lights) {
        json_["lights"].push_back(light->serialize());
    }

    return json_;
}