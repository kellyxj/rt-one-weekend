#include <iostream>
#include <fstream>
#include "vec4.h"

int main() {
    int nx = 200;
    int ny = 100;
    std::ofstream output("image.ppm");
    output << "P3\n" << nx << " " << ny << "\n255\n";
    for(int j = 0; j < ny; j++) {
        for(int i = 0; i < nx; i++) {
            float r = float(i)/float(nx);
            float g = float(j)/float(ny);
            float b = .2;
            int ir = int(255.99 * r);
            int ig = int (255.99 * g);
            int ib = int(255.99 * b);
            output << ir << " " << ig << " " << ib << "\n";
        }
    }
    vec4 test(1,2,3);
    std::cout << test.length();
    output.close();
}