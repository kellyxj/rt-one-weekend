#include "image.hpp"

Color Image::getPixel(int x, int y) {
    return this->pixels[y*width+x];
}
void Image::setPixel(int x, int y, Color c) {
    this->pixels[y*width+x] = c;
}
std::string Image::dump_ppm() {
    std::string output = "P3\n" + std::to_string(this->width) + " " + std::to_string(this->height) + "\n255\n";
    for(int j = this->height-1; j >= 0; j--) {
        for(int i = 0; i < this->width; i++) {
            Color c = this->getPixel(i,j);
            float r = c.channels[0];
            float g = c.channels[1];
            float b = c.channels[2];
            r = 255*r;
            g = 255*g;
            b = 255*b;

            r = r > 255 ? 255 : r;
            g = g > 255 ? 255 : g;
            b = b > 255 ? 255 : b;

            output += std::to_string(int(r)) + " " + std::to_string(int(g)) + " " + std::to_string(int(b)) + "\n";
        }
    }
    return output;
}