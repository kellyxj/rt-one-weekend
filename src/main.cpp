#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include "color.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "mat4.hpp"
#include "rayTracer.hpp"

int main() {
    srand(time(NULL));
    std::chrono::time_point<std::chrono::system_clock> start, end;
  
    start = std::chrono::system_clock::now();

    int nx = 400;
    int ny = 400;
    std::ofstream output("image.ppm");

    Scene scene;
    Plane plane;
    vec4 axis(0,0,1,0);
    Plane* plane_pointer = &plane;
    scene.items.push_back(plane_pointer);

    Camera cam(vec4(-2,0,1,1), 0, 0, nx, ny, .01, 45, 1);
    Camera* cam_pointer = &cam;
    scene.cameras.push_back(cam_pointer);
    RayTracer rayTracer;
    rayTracer.sampleRate = 16;

    Image image;
    image = rayTracer.takePicture(scene, 0);
    output << image.dump_ppm();
    
    end = std::chrono::system_clock::now();
  
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    output.close();
}