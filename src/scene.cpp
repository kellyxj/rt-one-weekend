#include "scene.hpp"
using namespace nlohmann;

json Scene::serialize() {
    json json_;
    json_ = {
        {"ambientLight", ambientLight},
        {"backgroundColor", {backgroundColor.r, backgroundColor.g, backgroundColor.b}}
    };
    return json_;
}

Scene Scene::deserialize(json json_) {

}