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
    int nx = 2;
    int ny = 2;

    RayTracer rayTracer;
    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 1;

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

    std::vector<RealisticCamera::LensElementInterface> lenses;
    // lenses.push_back(RealisticCamera::LensElementInterface(35.98738, 1.21638, 1.54, 23.716/2));
    lenses.push_back(RealisticCamera::LensElementInterface(11.69718, 9.9957, 1, 17.996/2));
    lenses.push_back(RealisticCamera::LensElementInterface(13.08714, 5.12622, 1.772, 12.364/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-22.63294, 1.76924, 1.617, 9.812/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(71.05802, 0.8184, 1, 9.152/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(0, 2.27766, 0, 8.756/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-9.58584, 2.435254, 1.617, 8.184/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-11.28864, 0.11506, 1, 9.152/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-166.7765, 3.09606, 1.713, 10.648/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-7.5911, 1.32682, 1.805, 11.44/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-16.7662, 3.98068, 1, 12.276/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-7.70286, 1.21638, 1.617, 13.42/2));
    // lenses.push_back(RealisticCamera::LensElementInterface(-11.98328, 20.0, 1, 17.996/2));
    RealisticCamera cam2(lenses, vec4(-3,0,0.5,1), 0, 0, nx, ny, 5.0); 
    cam2.gamma = 2;
    scene.cameras.push_back(&cam2);

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
    image = rayTracer.takePicture(scene, 2);
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