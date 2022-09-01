#include "image.hpp"

Color Image::getPixel(int x, int y) {
    return this->pixels[y*width+x];
}
void Image::setPixel(int x, int y, Color c) {
    this->pixels[y*width+x].r = c.r;
    this->pixels[y*width+x].g = c.g;
    this->pixels[y*width+x].b = c.b;
    this->pixels[y*width+x].a = c.a;
}
std::string Image::dump_ppm() {
    std::string output = "P3\n" + std::to_string(this->width) + " " + std::to_string(this->height) + "\n255\n";
    for(int j = this->height-1; j >= 0; j--) {
        for(int i = 0; i < this->width; i++) {
            Color c = this->getPixel(i,j);
            output += std::to_string((int)(255*c.r)) + " " + std::to_string((int)(255*c.g)) + " " + std::to_string((int)(255*c.b)) + "\n";
        }
    }
    return output;
}