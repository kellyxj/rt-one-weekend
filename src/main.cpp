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
#include "cameras/thinLensCamera.hpp"
#include "cameras/environmentCamera.hpp"
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
    std::stringstream ss;
    ss << start;
    std::string timestamp = ss.str();

    // * General scene settings
    // resolution
    int nx = 800;
    int ny = 800;

    RayTracer rayTracer;
    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 128;

    Scene scene;
    scene.name = "sphere";
    scene.ambientLight = 0.5;
    scene.backgroundColor = white;
    
    // * Scene Camera(s)
    ThinLensCamera cam0(vec4(-3, 0, 0.5, 1), 0, 0, nx, ny, 1, 90, nx/ny);
    cam0.gamma = 2;
    cam0.apertureRadius = 0.1;
    cam0.focalDistance = 3;
    scene.cameras.push_back(&cam0);

    EnvironmentCamera cam1(vec4(-3, 0, 0.5, 1), nx, ny, 180, 0);
    cam1.gamma = 2;
    scene.cameras.push_back(&cam1);

    // generic vec4 names
    vec4 translate;
    vec4 axis;
    vec4 scaleAmount;


    // * Scene materials
    groundGrid planeMat;
    
    Glass glass;
    glass.c = white;
    glass.n_i = .9;

    Mirror mirror;
    mirror.c = white;
    mirror.r0 = .80;

    base whiteMat;
    whiteMat.c = white;

    base redMat;
    redMat.c = red;
    // redMat.brightness = 10;

    base greenMat;
    greenMat.c = green;

    base blueMat;
    blueMat.c = blue;


    // * Scene lights
    base *lightMat = new base();
    lightMat->c = white;
    lightMat->brightness = 10;

    RectangleLight light(.5, .5);
    light.setMaterial(*lightMat);
    translate = vec4(0, 0, 1.99, 0);
    light.translate(translate);
    // scene.lights.push_back(&light);


    // * Scene geometry
    Plane groundPlane;
    groundPlane.setMaterial(planeMat);
    scene.items.push_back(&groundPlane);
/*
    // Cornell box, dimensions (0:2, -1:1, -1:1)
    // This takes waaay longer to render since light needs to keep bouncing to recursion limit
    Plane bottomPlane;
    bottomPlane.setMaterial(whiteMat);
    scene.items.push_back(&bottomPlane);

    Plane topPlane;
    topPlane.setMaterial(whiteMat);
    translate = vec4(0, 0, 2.0, 0);
    topPlane.translate(translate);
    scene.items.push_back(&topPlane);

    Plane frontPlane;
    frontPlane.setMaterial(whiteMat);
    vec4 yAxis(0, 1, 0, 0);
    frontPlane.rotate(90, yAxis);
    translate = vec4(1, 0, 0, 0);
    frontPlane.translate(translate);
    scene.items.push_back(&frontPlane);

    Plane backPlane;
    backPlane.setMaterial(whiteMat);
    backPlane.rotate(90, yAxis);
    translate = vec4(-1, 0, 0, 0);
    backPlane.translate(translate);
    scene.items.push_back(&backPlane);

    Plane rightPlane;
    rightPlane.setMaterial(greenMat);
    vec4 xAxis(1, 0, 0, 0);
    rightPlane.rotate(90, xAxis);
    translate = vec4(0, 1, 0, 0);
    rightPlane.translate(translate);
    scene.items.push_back(&rightPlane);

    Plane leftPlane;
    leftPlane.setMaterial(redMat);
    leftPlane.rotate(90, xAxis);
    translate = vec4(0, -1, 0, 0);
    leftPlane.translate(translate);
    scene.items.push_back(&leftPlane);
*/

    // Other scene objects
    Sphere sphere0;
    translate = vec4(-1, 1, 0.5, 0);
    sphere0.translate(translate);
    scaleAmount = vec4(0.5, 0.5, 0.5);
    sphere0.scale(scaleAmount);
    sphere0.setMaterial(blueMat);
    scene.items.push_back(&sphere0);

    Sphere sphere1;
    translate = vec4(0, 0, 0.5, 0);
    sphere1.translate(translate);
    scaleAmount = vec4(0.5, 0.5, 0.5);
    sphere1.scale(scaleAmount);
    sphere1.setMaterial(greenMat);
    scene.items.push_back(&sphere1);

    Sphere sphere2;
    translate = vec4(1, -1, 0.5, 0);
    sphere2.translate(translate);
    scaleAmount = vec4(0.5, 0.5, 0.5);
    sphere2.scale(scaleAmount);
    sphere2.setMaterial(redMat);
    scene.items.push_back(&sphere2);


    // * Save scene information...
    json json_ = scene.serialize();
    SceneLoader sceneLoader;
    sceneLoader.createSceneFile(json_);
    sceneLoader.writeJson(json_);

    // create output file
    std::string path = "../data/" + scene.name + "/renders/" + timestamp + ".ppm";
    std::ofstream output(path);
    //std::cout << json_;
    //scene = scene.deserialize(sceneLoader.readJson(scene.name));
    //std::cout << scene.serialize().dump(4);


    // * Create and save ray traced image
    Image image(nx, ny);
    image = rayTracer.takePicture(scene, 0);
    output << image.dump_ppm();

/*  
    // Testing helper
    vec4 origin(0, 0, .5, 1);
    vec4 direction(1, .01, 0, 0);
    ray inRay(origin, direction);
    testScatter(inRay, pos, normal, &glass);
    testHit(inRay, &triangle);
*/

    // Print scene render statistics (e.g. time)
    end = std::chrono::system_clock::now();

    std::chrono::duration<float> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    output.close();
}