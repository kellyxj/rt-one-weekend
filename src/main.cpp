#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include "color.hpp"
#include "math/ray.hpp"
#include "camera.hpp"
#include "math/mat4.hpp"
#include "renderers/rayTracer.hpp"
#include "util/date.h"
#include "util/scene_loader.hpp"

using namespace date;
using namespace std::chrono;

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

    int nx = 1600;
    int ny = 1600;
    std::stringstream ss;
    ss << start;

    std::string timestamp = ss.str();

    Scene scene;
    scene.name = "cornell_box_sphere";
    scene.ambientLight = 0;
    scene.backgroundColor = black;

    std::string path = "../data/" + scene.name + "/renders/" + timestamp + ".ppm";
    std::ofstream output(path);

    Plane plane;
    groundGrid planeMat;

    //base sphereMat;
    Glass glass;
    Mirror mirror;

    mirror.c = white;
    glass.c = Color(.9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1), .9 + .1 * (rand() % 1));
    mirror.r0 = .80;
    glass.n_i = 1.6;

    plane.setMaterial(planeMat);

    vec4 axis(0,0,.5,0);
    vec4 translate(0,0,2,0);
    //plane.rotate(30, axis);
    //plane.translate(translate);
    Plane* plane_pointer = &plane;
    scene.items.push_back(plane_pointer);

    vec4 scaleAmount(.5, .5, .5);

    RectangleLight light(1, 1, 6);

    base lightMat;
    lightMat.c = white;
    light.c = white;

    light.setMaterial(lightMat);
    light.translate(translate);
    Light * lightPointer = &light;
    scene.lights.push_back(lightPointer);

    Sphere sphere;
    sphere.scale(scaleAmount);
    sphere.translate(axis);

    base sphereMat;
    sphereMat.c = red;
    //sphereMat.brightness = 10;
    sphere.setMaterial(glass);

    Sphere* sphere_pointer = &sphere;
    scene.items.push_back(sphere_pointer);
    
    Mesh mesh;
    mesh.loadFromObj("cornell_box.obj");

    axis = vec4(1,0,0);
    mesh.rotate(90, axis);

    axis = vec4(0,0,1);
    mesh.rotate(-90, axis);

    vec4 scaleVec(2,2,2);
    //mesh.scale(scaleVec);
    translate = vec4(0,0,1);
    //mesh.translate(translate);

    base meshMaterial;
    meshMaterial.c = grey;
    //mesh.setMaterial(meshMaterial);
    mesh.constructBVH();

    Mesh * mesh_pointer = &mesh;
    //scene.items.push_back(mesh_pointer);

    Camera cam(vec4(-2,0,1,1), 0, 0, nx, ny, .01, 90, 1);
    cam.gamma = 2;
    Camera* cam_pointer = &cam;
    scene.cameras.push_back(cam_pointer);
    RayTracer rayTracer;
    rayTracer.maxDepth = 8;
    rayTracer.sampleRate = 256;

    json json_ = scene.serialize();
    SceneLoader sceneLoader;
    sceneLoader.createSceneFile(json_);

    Image image;
    //image = dynamic_cast<PathTracer*>(&rayTracer)->takePicture(scene, 0);
    image = rayTracer.takePicture(scene,0);
    output << image.dump_ppm();

    vec4 origin(0,0,.5,1);
    vec4 direction(1,.01,0,0);
    ray inRay(origin, direction);
    //testScatter(inRay, pos, normal, &glass);
    //testHit(inRay, &triangle);
    
    end = std::chrono::system_clock::now();
  
    std::chrono::duration<float> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    output.close();
}