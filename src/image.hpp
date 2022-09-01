#include "color.hpp"
#include <vector>
#include <string>

class Image {
public:
    int width;
    int height;
    std::vector<Color> pixels;

    //bottom left: (0,0). top right: (width-1, height-1)
    Color getPixel(int x,int y);
    void setPixel(int x, int y, Color c);

    std::string dump_ppm();
};