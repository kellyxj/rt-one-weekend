#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include "color.hpp"
#include "math/ray.hpp"
#include "cameras/pinholeCamera.hpp"
#include "math/mat4.hpp"
#include "renderers/rayTracer.hpp"
#include "renderers/spectralRenderer.hpp"
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

    scene.ambientLight = .75;
    scene.backgroundColor = Color(.75);

    Square square;
    groundCheckerboard planeMat;
    // planeMat.c = white;

    // base sphereMat;
    Glass glass;
    Mirror mirror;

    mirror.c = white;
    glass.c = white;
    // Color(.9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    //  .9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
    //.9 + .1 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    mirror.r0 = .80;
    Color c;
    for (int i = 0; i < c.nSpectralSamples; i++)
    {
        glass.n_i.spectrum.channels[i] = 1 + 0.5 - c.getLambda(i) * 0.001;
    }
    // glass.n_i.spectrum.channels[0] = 1.11;
    // glass.n_i.spectrum.channels[1] = 1.1;
    // glass.n_i.spectrum.channels[2] = 1.09;

    vec4 scale(4, 4, 4, 0);
    vec4 axis(1, 0, 0, 0);
    vec4 translate(0, 0, -.5, 0);
    Plane plane;
    // plane.rotate(90, axis);
    plane.scale(scale);
    plane.setMaterial(planeMat);
    plane.translate(translate);
    scene.items.push_back(&plane);

    // square.setMaterial(planeMat);
    // square.translate(translate);
    // square.scale(scale);
    // scene.items.push_back(&square);

    // Square square2;
    // square2.setMaterial(planeMat);
    // square2.rotate(90, axis);
    // translate = vec4(0, -2, 1, 0);
    // square2.translate(translate);
    // square2.scale(scale);
    // scene.items.push_back(&square2);

    // Square square3;
    // square3.setMaterial(planeMat);
    // square3.rotate(90, axis);
    // translate = vec4(0, 2, 1, 0);
    // square3.translate(translate);
    // square3.scale(scale);
    // scene.items.push_back(&square3);

    // Square square4;
    // square4.setMaterial(planeMat);
    // translate = vec4(0, 0, 3, 0);
    // square4.translate(translate);
    // square4.scale(scale);
    // scene.items.push_back(&square4);

    // Square square5;
    // square5.setMaterial(planeMat);
    // axis = vec4(0, 1, 0, 0);
    // square5.rotate(90, axis);
    // translate = vec4(2, 0, 1, 0);
    // square5.translate(translate);
    // square5.scale(scale);
    // scene.items.push_back(&square5);

    vec4 scaleAmount(.5, .5, .5);

    RectangleLight light(.1, .1);

    // base *lightMat = new base();
    // lightMat->c = white;
    // lightMat->brightness = 5;

    // light.setMaterial(*lightMat);
    // // light.rotate(-90, axis);
    // translate = vec4(0, 0, 1.9, 0);
    // light.translate(translate);
    // scene.lights.push_back(&light);

    Sphere sphere;
    // sphere.translate(axis);
    sphere.scale(scaleAmount);
    Film film;
    film.c = white;
    base sphereMat;
    sphereMat.c = Color(46.0 / 255, 174.0 / 255, 144.0 / 255);

    sphere.setMaterial(film);

    // scene.items.push_back(&sphere);

    // Mesh mesh;
    // mesh.loadFromObj("model.obj");

    // axis = vec4(0, 1, 0, 0);
    // mesh.rotate(-20, axis);

    // axis = vec4(0, 0, 1, 0);
    // mesh.rotate(90, axis);

    // // axis = vec4(0, 1, 0, 0);
    // // mesh.rotate(-90, axis);

    // vec4 scaleVec(.5, 1, .5, 0);
    // mesh.scale(scaleVec);
    translate = vec4(0, 0, 1.5, 0);
    sphere.translate(translate);

    // base meshMaterial;
    // meshMaterial.c = Color(0);
    // meshMaterial.c.channels[0] = 0.5;
    // mirror.c = Color(.6);
    // mesh.setMaterial(meshMaterial);
    // mesh.constructBVH();

    scene.items.push_back(&sphere);

    Sphere sphere2;
    sphere2.translate(translate);
    scaleAmount = vec4(.48, .48, .48);
    sphere2.scale(scaleAmount);
    sphere2.setMaterial(sphereMat);
    scene.items.push_back(&sphere2);

    Sphere sphere3;
    translate = vec4(1, 0, 0, 0);
    sphere3.scale(scaleAmount);
    sphere3.setMaterial(sphereMat);
    scene.items.push_back(&sphere3);

    PinholeCamera cam(vec4(0, 2, 1, 1), 90, 0, nx, ny, .01, 90, 1);
    cam.gamma = 2;
    scene.cameras.push_back((Camera *)&cam);
    SpectralRenderer rayTracer;

    rayTracer.maxDepth = 10;
    rayTracer.sampleRate = 64;

    json json_ = scene.serialize();
    SceneLoader sceneLoader;
    sceneLoader.createSceneFile(json_);
    sceneLoader.writeJson(json_);

    std::string path = "../data/" + scene.name + "/renders/" + timestamp + ".ppm";
    std::ofstream output(path);

    // std::cout << json_;

    // scene = scene.deserialize(sceneLoader.readJson(scene.name));
    // std::cout << scene.serialize().dump(4);

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