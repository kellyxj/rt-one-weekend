#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "color.h"
#include "ray.h"
#include "camera.h"

int main() {
    srand(time(NULL));

    int nx = 400;
    int ny = 400;
    std::ofstream output("image.ppm");
    output << "P3\n" << nx << " " << ny << "\n255\n";
    for(int j = 0; j < ny; j++) {
        for(int i = 0; i < nx; i++) {
            color c(int(255*(i+j)/(nx+ny)), int(255*(i+j)/(nx+ny)), int(255*(i+j)/(nx+ny)));
            output << c.r << " " << c.g << " " << c.b << "\n";
        }
    }
    output.close();

    Camera cam;
    std::cout << cam.getEyeRay(512, 512).origin << cam.getEyeRay(512, 512).direction;
}