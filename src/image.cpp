#include "image.hpp"

color Image::getPixel(int x, int y) {
    return this->pixels[y*width+x];
}
void Image::setPixel(int x, int y, color c) {
    this->pixels[y*width+x].r = c.r;
    this->pixels[y*width+x].g = c.g;
    this->pixels[y*width+x].b = c.b;
    this->pixels[y*width+x].a = c.a;
}
std::string Image::dump_ppm() {
    std::string output = "P3\n" + std::to_string(this->width) + " " + std::to_string(this->height) + "\n255\n";
    for(color c : this->pixels) {
        output += std::to_string((int)c.r) + " " + std::to_string((int)c.g) + " " + std::to_string((int)c.b) + "\n";
    }
    return output;
}