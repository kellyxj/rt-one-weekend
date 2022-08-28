#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "color.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "mat4.hpp"
#include "plane.hpp"

int main() {
    srand(time(NULL));

    int nx = 400;
    int ny = 400;
    std::ofstream output("image.ppm");

    output << "P3\n" << nx << " " << ny << "\n255\n";
    Plane plane;

    ray inRay(vec4(0,0,1,0), vec4(1,0,-.1,0), 1);
    Hit hit = plane.trace(inRay);
    std::cout << hit.pos;

    for(int j = 0; j < ny; j++) {
        for(int i = 0; i < nx; i++) {
            color c(int(255*(i+j)/(nx+ny)), int(255*(i+j)/(nx+ny)), int(255*(i+j)/(nx+ny)));
            output << c.r << " " << c.g << " " << c.b << "\n";
        }
    }
    output.close();
}