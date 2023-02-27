#include "color.hpp"

json Color::serialize() {
    json json_ {
        {"red", r},
        {"green", g},
        {"blue", b},
        {"alpha", a}
    };
    return json_;
}
Color Color::deserialize(json json_){
    r = json_["red"];
    g = json_["green"];
    b = json_["blue"];
    a = json_["alpha"];
    return *this;
}