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

void testScatter(ray & inRay, vec4 & pos, vec4 & normal, Material* material) {
    ray outRay = material->scatter(inRay, pos, normal);
    std::cout << "Incident ray " << inRay;
    std::cout << "Outgoing ray " << outRay;
}

void testHit(ray & inRay, Geometry* hittable) {
    Hit hit = hittable->trace(inRay);
    std::cout << hit;
}

int main() {
    srand(time(NULL));
    std::chrono::time_point<std::chrono::system_clock> start, end;
  
    start = std::chrono::system_clock::now();

    int nx = 400;
    int ny = 400;
    std::ofstream output("../data/image.ppm");

    Scene scene;
    scene.ambientLight = .25;

    Plane plane;
    groundGrid planeMat;

    base sphereMat;
    Glass glass;
    Mirror mirror;

    mirror.c = Color(.9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1));
    glass.c = Color(.9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1));
    glass.n_i = 2;
    sphereMat.c = red;

    plane.setMaterial(planeMat);

    vec4 axis(0,0,1,0);
    vec4 translate(0,0,3,0);
    //plane.rotate(30, axis);
    Plane* plane_pointer = &plane;
    scene.items.push_back(plane_pointer);

    Square square;
    base squareMat;
    squareMat.c = white;
    squareMat.brightness = 1;
    square.setMaterial(squareMat);

    square.rotate(90, axis);
    square.translate(translate);
    Square* square_pointer = &square;
    scene.items.push_back(square_pointer);

    Sphere sphere;
    sphere.translate(axis);

    //vec4 scale(2,2,2);
    //sphere.scale(scale);
    sphere.setMaterial(glass);

    Sphere* sphere_pointer = &sphere;
    scene.items.push_back(sphere_pointer);

    Camera cam(vec4(-3,0,.5,1), 0, 0, nx, ny, .01, 90, 1);
    cam.gamma = 2;
    Camera* cam_pointer = &cam;
    scene.cameras.push_back(cam_pointer);
    RayTracer rayTracer;
    rayTracer.maxDepth = 3;
    rayTracer.sampleRate = 64;

    Image image;
    //image = dynamic_cast<PathTracer*>(&rayTracer)->takePicture(scene, 0);
    //image = rayTracer.takePicture(scene,0);
    output << image.dump_ppm();

    vec4 origin(0,0,1,1);
    vec4 direction(0,1,-1,0);
    ray inRay(origin, direction);
    
    vec4 a(-1,-1,0,1);
    vec4 b(1,-1,0,1);
    vec4 c(0,1,0,1);

    Triangle triangle(a,b,c);
    triangle.setMaterial(sphereMat);
    Hit hit = triangle.trace(inRay);
    std::cout << hit;
    //testScatter(inRay, pos, normal, &glass);
    //testHit(inRay, &plane);
    
    end = std::chrono::system_clock::now();
  
    std::chrono::duration<float> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    output.close();
}