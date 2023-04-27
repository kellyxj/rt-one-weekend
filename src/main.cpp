#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include "color.hpp"
#include "math/ray.hpp"
#include "cameras/pinholeCamera.hpp"
#include "cameras/realisticCamera.hpp"
#include "math/mat4.hpp"
#include "renderers/rayTracer.hpp"
#include "util/date.h"
#include "util/scene_loader.hpp"

using namespace date;
using namespace std::chrono;

void testScatter(ray &inRay, vec4 &pos, vec4 &normal, Material *material)
{
    ray outRay = material->scatter(inRay, pos, normal);
    std::cout << "Incident ray " << inRay;
    std::cout << "Outgoing ray " << outRay;
}

void testHit(ray &inRay, Geometry *hittable)
{
    Hit hit = hittable->trace(inRay);
    std::cout << hit;
}

int main()
{
    srand(time(NULL));
    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    int nx = 400;
    int ny = 400;
    std::stringstream ss;
    ss << start;

    std::string timestamp = ss.str();

    Scene scene;
    scene.name = "sphere";

    scene.ambientLight = .5;
    scene.backgroundColor = white;

    Plane plane;
    groundGrid planeMat;

    //base sphereMat;
    Glass glass;
    Mirror mirror;

    mirror.c = white;
    glass.c = Color(.9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
        .9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
        .9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    mirror.r0 = .80;
    glass.n_i = .9;

    plane.setMaterial(planeMat);

    vec4 axis(0, 0, .5, 0);
    vec4 translate(0, 0, 1.99, 0);
    // plane.rotate(30, axis);
    // plane.translate(translate);
    scene.items.push_back(&plane);

    vec4 scaleAmount(.5, .5, .5);

    RectangleLight light(.5, .5);

    base *lightMat = new base();
    lightMat->c = white;
    lightMat->brightness = .5;

    light.setMaterial(*lightMat);
    light.translate(translate);
    //scene.lights.push_back(&light);

    Sphere sphere;
    sphere.translate(axis);
    sphere.scale(scaleAmount);

    base sphereMat;
    sphereMat.c = red;
    // sphereMat.brightness = 10;
    sphere.setMaterial(glass);

    scene.items.push_back(&sphere);

    Mesh mesh;
    //mesh.loadFromObj("cornell_box_cube.obj");

    axis = vec4(1, 0, 0);
    mesh.rotate(90, axis);

    axis = vec4(0, 0, 1);
    mesh.rotate(-90, axis);

    vec4 scaleVec(2, 2, 2);
    // mesh.scale(scaleVec);
    translate = vec4(0, 0, 1);
    // mesh.translate(translate);

    base meshMaterial;
    meshMaterial.c = grey;
    mirror.c = Color(.6, .8, .6);
    //mesh.setMaterial(mirror);
    mesh.constructBVH();

    //scene.items.push_back(&mesh);

    RealisticCamera cam(vec4(-2, 0, 1, 1), 0, 0, nx, ny, 1, 90, 1);
    cam.gamma = 2;
    cam.apertureSize = .01;
    scene.cameras.push_back(&cam);
    RayTracer rayTracer;

    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 64;

    json json_ = scene.serialize();
    SceneLoader sceneLoader;
    sceneLoader.createSceneFile(json_);
    sceneLoader.writeJson(json_);

    std::string path = "../data/" + scene.name + "/renders/" + timestamp + ".ppm";
    std::ofstream output(path);

    //std::cout << json_;

    //scene = scene.deserialize(sceneLoader.readJson(scene.name));
    //std::cout << scene.serialize().dump(4);

    Image image(nx, ny);
    // image = dynamic_cast<PathTracer*>(&rayTracer)->takePicture(scene, 0);
    image = rayTracer.takePicture(scene, 0);
    output << image.dump_ppm();

    vec4 origin(0, 0, .5, 1);
    vec4 direction(1, .01, 0, 0);
    ray inRay(origin, direction);
    // testScatter(inRay, pos, normal, &glass);
    // testHit(inRay, &triangle);

    end = std::chrono::system_clock::now();

    std::chrono::duration<float> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    output.close();
}