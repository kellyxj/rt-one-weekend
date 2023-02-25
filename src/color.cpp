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

}