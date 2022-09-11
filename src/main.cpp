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
#include "renderers/rayTracer.hpp"

int main() {
    srand(time(NULL));
    std::chrono::time_point<std::chrono::system_clock> start, end;
  
    start = std::chrono::system_clock::now();

    int nx = 400;
    int ny = 400;
    std::ofstream output("image.ppm");

    Scene scene;
    Plane plane;
    groundGrid planeMat;
    plane.setMaterial(planeMat);

    vec4 axis(0,0,2,0);
    //plane.rotate(30, axis);
    Plane* plane_pointer = &plane;
    scene.items.push_back(plane_pointer);

    Square square;
    square.setMaterial(planeMat);
    Square* square_pointer = &square;
    //scene.items.push_back(square_pointer);

    Sphere sphere;
    sphere.translate(axis);
    base sphereMat;
    sphereMat.c = red;
    sphere.setMaterial(sphereMat);

    Sphere* sphere_pointer = &sphere;
    scene.items.push_back(sphere_pointer);

    base lightMat;
    lightMat.c = white;
    SphereLight light;
    light.brightness = .85;
    light.setMaterial(lightMat);
    
    axis.z = 1000;
    light.translate(axis);

    vec4 scaleVec(200,200,200);
    light.scale(scaleVec);
    scene.lights.push_back(&light);

    SphereLight light2;
    light2.setMaterial(lightMat);
    light2.brightness = .4;
    axis.z = 3;
    axis.y = 3;
    axis.x = 0;
    scaleVec = vec4(.4,.4,.4);
    light2.translate(axis);
    light2.scale(scaleVec);
    //scene.lights.push_back(&light2);

    Camera cam(vec4(-3,0,1,1), 0, 0, nx, ny, .01, 90, 1);
    Camera* cam_pointer = &cam;
    scene.cameras.push_back(cam_pointer);
    RayTracer rayTracer;
    rayTracer.sampleRate = 4;

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