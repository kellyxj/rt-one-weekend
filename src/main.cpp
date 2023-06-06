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
    int nx = 400;
    int ny = 400;

    RayTracer rayTracer;
    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 16;

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

    float sensorSize = 50.0;
    std::vector<RealisticCamera::LensElementInterface> lenses;

    // * Small aperture spherical lens
    // lenses.push_back(RealisticCamera::LensElementInterface(50.0, 100.0, 1.29, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-50.0, 80.0, 1.0, 2.0));
    // sensorSize = 34.0;

    // * Large aperture spherical lens
    // lenses.push_back(RealisticCamera::LensElementInterface(50.0, 100.0, 1.29, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-50.0, 80.0, 1.0, 20.0));
    // sensorSize = 75.0;

    // * Biconvex lens
    // lenses.push_back(RealisticCamera::LensElementInterface(110.0, 20.0, 1.41, 100.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-110.0, 180.5, 1.0, 10.0));
    // sensorSize = 1000.0;

    // * Biconvex lens with aperture stop
    lenses.push_back(RealisticCamera::LensElementInterface(110.0, 10.0, 1.41, 100.0));
    lenses.push_back(RealisticCamera::LensElementInterface(0, 10.0, 0, 50.0));
    lenses.push_back(RealisticCamera::LensElementInterface(-110.0, 180.5, 1.0, 10.0));
    sensorSize = 1000.0;

    // * Biconcave lens
    // lenses.push_back(RealisticCamera::LensElementInterface(-110.0, 20.0, 1.41, 100.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(110.0, 140.0, 1.0, 1.0)); // Note the small aperture!
    // sensorSize = 500.0;

    // * PBRT Wide angle lens (22mm)
    // lenses.push_back(RealisticCamera::LensElementInterface(35.98738, 1.21638, 1.54, 23.716));
    // lenses.push_back(RealisticCamera::LensElementInterface(11.69718, 9.9957, 1, 17.996));
    // lenses.push_back(RealisticCamera::LensElementInterface(13.08714, 5.12622, 1.772, 12.364));
    // lenses.push_back(RealisticCamera::LensElementInterface(-22.63294, 1.76924, 1.617, 9.812));
    // lenses.push_back(RealisticCamera::LensElementInterface(71.05802, 0.8184, 1, 9.152));
    // lenses.push_back(RealisticCamera::LensElementInterface(0, 2.27766, 0, 8.756));
    // lenses.push_back(RealisticCamera::LensElementInterface(-9.58584, 2.435254, 1.617, 8.184));
    // lenses.push_back(RealisticCamera::LensElementInterface(-11.28864, 0.11506, 1, 9.152));
    // lenses.push_back(RealisticCamera::LensElementInterface(-166.7765, 3.09606, 1.713, 10.648));
    // lenses.push_back(RealisticCamera::LensElementInterface(-7.5911, 1.32682, 1.805, 11.44));
    // lenses.push_back(RealisticCamera::LensElementInterface(-16.7662, 3.98068, 1, 12.276));
    // lenses.push_back(RealisticCamera::LensElementInterface(-7.70286, 1.21638, 1.617, 13.42));
    // lenses.push_back(RealisticCamera::LensElementInterface(-11.98328, 11.0, 1.0, 17.996));
    // sensorSize = 40.0;

    // * Gaussian lens (50 mm)
    // lenses.push_back(RealisticCamera::LensElementInterface(29.475, 3.76, 1.67, 25.2));
    // lenses.push_back(RealisticCamera::LensElementInterface(84.83, 0.12, 1.0, 25.2));
    // lenses.push_back(RealisticCamera::LensElementInterface(19.275, 4.025, 1.67, 23));
    // lenses.push_back(RealisticCamera::LensElementInterface(40.77, 3.275, 1.699, 23.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(12.75, 5.705, 1.0, 18.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(0, 4.5, 0, 17.1));
    // lenses.push_back(RealisticCamera::LensElementInterface(-14.495, 1.18, 1.603, 17.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(40.77, 6.065, 1.658, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-20.385, 0.19, 1.0, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(437.065, 3.22, 1.717, 20.0));
    // lenses.push_back(RealisticCamera::LensElementInterface(-39.73, 36.77, 1.0, 20.0));
    // sensorSize = 78.0;

    // * Fisheye lens (16 mm, 155.9 FOV)
    // lenses.push_back(RealisticCamera::LensElementInterface(30.2249, 0.8335, 1.62, 30.34));
    // lenses.push_back(RealisticCamera::LensElementInterface(11.3931, 7.4136, 1, 20.68));
    // lenses.push_back(RealisticCamera::LensElementInterface(75.2019, 1.0654, 1.639, 17.8));
    // lenses.push_back(RealisticCamera::LensElementInterface(8.3349, 11.1549, 1, 13.42));
    // lenses.push_back(RealisticCamera::LensElementInterface(9.5882, 2.0054, 1.654, 9.02));
    // lenses.push_back(RealisticCamera::LensElementInterface(43.8677, 5.3895, 1, 8.14));
    // lenses.push_back(RealisticCamera::LensElementInterface(0, 1.4163, 0, 6.08));
    // lenses.push_back(RealisticCamera::LensElementInterface(29.4541, 2.1934, 1.517, 5.96));
    // lenses.push_back(RealisticCamera::LensElementInterface(-5.2265, 0.9714, 1.805, 5.84));
    // lenses.push_back(RealisticCamera::LensElementInterface(-14.2884, 0.0627, 1, 5.96));
    // lenses.push_back(RealisticCamera::LensElementInterface(-22.3726, 0.94, 1.673, 5.96));
    // lenses.push_back(RealisticCamera::LensElementInterface(-15.0404, 23.0, 1, 6.52));
    // sensorSize = 38.0;

    RealisticCamera cam3(lenses, vec4(-3,0,0.5,1), 180, 0, nx, ny, sensorSize, 3.0); 
    cam3.gamma = 2;
    cam3.flipImage = false;
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