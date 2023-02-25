#include "scene.hpp"

json Scene::serialize() {
    json json_ = {
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

}