#include "color.hpp"

Color Color::operator + (const Color & c){
    int size = channels.size();
    Color output;
    for(int i = 0; i < size; i++) {
        output.channels[i] = channels[i] + c.channels[i];
    }
    return output;
}
Color & Color::operator += (const Color & c) {
    int size = channels.size();
    for(int i = 0; i < size; i++) {
        channels[i] += c.channels[i];
    }
    return (*this);
}
Color Color::operator - (const Color & c) {
    int size = channels.size();
    Color output;
    for(int i = 0; i < size; i++) {
        output.channels[i] = channels[i] - c.channels[i];
    }
    return output;
}
Color & Color::operator -= (const Color & c) {
    int size = channels.size();
    for(int i = 0; i < size; i++) {
        channels[i] -= c.channels[i];
    }
    return (*this);
}
Color Color::operator * (float c){
    int size = channels.size();
    Color output;
    for(int i = 0; i < size; i++) {
        output.channels[i] = channels[i] * c;
    }
    return output;
}
Color & Color::operator *= (float c){
    int size = channels.size();
    for(int i = 0; i < size; i++) {
        channels[i] *= c;
    }
    return (*this);
}
Color Color::operator * (const Color & c){
    int size = channels.size();
    Color output;
    for(int i = 0; i < size; i++) {
        output.channels[i] = channels[i] * c.channels[i];
    }
    return output;
}
Color & Color::operator *= (const Color & c){
    int size = channels.size();
    for(int i = 0; i < size; i++) {
        channels[i] *= c.channels[i];
    }
    return (*this);
}
Color Color::operator / (float c) {
    int size = channels.size();
    Color output;
    for(int i = 0; i < size; i++) {
        output.channels[i] = channels[i] / c;
    }
    return output;
}
Color & Color::operator /= (float c){
    int size = channels.size();
    for(int i = 0; i < size; i++) {
        channels[i] /= c;
    }
    return (*this);
}

json Color::serialize() {
    json json_ {
        {"entries", channels}
    };
    return json_;
}
Color Color::deserialize(json json_){
    channels = json_["entries"].get<std::vector<float>>();
    return *this;
}