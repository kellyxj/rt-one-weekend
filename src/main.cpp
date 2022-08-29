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
    Camera cam(vec4(-2,0,1,1), 30, 0, nx, ny, .01, 90, 1);

    //scan from top to bottom, left to right in accordance with .ppm format
    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            ray eyeRay = cam.getEyeRay(i+.5, j+.5);
            Hit hit;
            hit = plane.trace(eyeRay);
            if(hit.t < 1e10) {
                double xPos = hit.pos.x;
                double yPos = hit.pos.y;
                double intPart;
                if(modf(xPos, &intPart) > .9 || modf(xPos, &intPart) < -.9 || modf(yPos, &intPart) > .9 || modf(yPos, &intPart) < -.9) {
                    color c(0, 255, 0);
                    output << c.r << " " << c.g << " " << c.b << "\n";
                }
                else if((modf(xPos, &intPart) > 0 && modf(xPos, &intPart) < .1) || (modf(yPos, &intPart) > 0 && modf(yPos, &intPart) < .1) || 
                        (modf(xPos, &intPart) < 0 && modf(xPos, &intPart) > -.1) || (modf(yPos, &intPart) < 0 && modf(yPos, &intPart) > -.1)) 
                {
                    color c(0, 255, 0);
                    output << c.r << " " << c.g << " " << c.b << "\n";
                }
                else {
                    color c(255, 255, 255);
                    output << c.r << " " << c.g << " " << c.b << "\n";
                }  
            }
            else {
                color c(0, 0, 255);
                output << c.r << " " << c.g << " " << c.b << "\n";
            }
        }
    }
    output.close();
}