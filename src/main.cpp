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
    int nx = 1;
    int ny = 1;

    RayTracer rayTracer;
    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 1;

    Scene scene;
    scene.name = "sphere";
    scene.ambientLight = 0.5;
    scene.backgroundColor = white;
    
    // * Scene Camera(s)
    EnvironmentCamera cam0(vec4(-3, 0, 0.5, 1), nx, ny, 180, 0);
    cam0.gamma = 2;
    scene.cameras.push_back(&cam0);

    PinholeCamera cam1(vec4(-3, 0, 0.5, 1), 0, 0, nx, ny, 1, 90, nx/ny);
    cam1.gamma = 2;
    scene.cameras.push_back(&cam1);

    ThinLensCamera cam2(vec4(-3, 0, 0.5, 1), 0, 0, nx, ny, 1, 90, nx/ny);
    cam2.gamma = 2;
    cam2.apertureRadius = 0.1;
    cam2.focalDistance = 3;
    scene.cameras.push_back(&cam2);

    std::vector<RealisticCamera::LensElementInterface> lenses;
    lenses.push_back(RealisticCamera::LensElementInterface(50.0, 50.0, 1.0, 2.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-50.0, 100.0, 1.29, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(50.0, 50.0, 1.0, 2.0));
    RealisticCamera cam3(lenses, vec4(-3,0,0.5,1), 0, 0, nx, ny, 5.0); 
    cam3.gamma = 2;
    scene.cameras.push_back(&cam3);

    // generic vec4 names
    vec4 translate;
    vec4 axis;
    vec4 scaleAmount;


    // * Scene materials
    groundGrid planeMat;
    
    Glass glass;
    glass.c = white;
    glass.n_i = .9;

    Glass lens;
    lens.c = white;
    lens.n_i = 1.0;

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

    // Sphere lensSphere;
    // translate = vec4(-1, 0, 0.5, 1);
    // lensSphere.translate(translate);
    // lensSphere.setMaterial(lens);
    // scene.items.push_back(&lensSphere);

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
    image = rayTracer.takePicture(scene, 3);
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