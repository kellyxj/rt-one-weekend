#include "scene.hpp"

json Scene::serialize() {
    json json_ = {
        {"name", name},
        {"ambientLight", ambientLight},
        {"backgroundColor", backgroundColor.serialize()},
        {"items", json::array()},
        {"cameras",json::array()}
    };
    for(Camera* camera : cameras) {
        json_["cameras"].push_back(camera->serialize());
    }
    for(Geometry* item: items) {
        json_["items"].push_back(item->serialize());
    }
    for(Light* light: lights) {
        json_["lights"].push_back(light->serialize());
    }

    return json_;
}

Scene Scene::deserialize(json json_) {
    Scene scene;
    scene.name = json_["name"];
    scene.ambientLight = json_["ambientLight"];

    Color backgroundColor;
    auto backgroundColor_ = json_["backgroundColor"];
    backgroundColor = backgroundColor.deserialize(backgroundColor_);

    scene.backgroundColor = backgroundColor.deserialize(json_["backgroundColor"]);

    for(auto camera : json_["cameras"]) {
        Camera* cam = new Camera();
        *cam = cam->deserialize(camera);

        scene.cameras.push_back(cam);
    }
    
    for(auto item : json_["items"]) {
        int type_ = item["type"];
        GeometryType type = (GeometryType)type_;
        switch(type) {
            case GeometryType::plane: {
                Plane* plane = new Plane();
                plane = dynamic_cast<Plane*>(plane->deserialize(item));
                scene.items.push_back(plane);
                break;
            }
            case GeometryType::sphere: {
                Sphere* sphere = new Sphere();
                sphere = dynamic_cast<Sphere*>(sphere->deserialize(item));
                scene.items.push_back(sphere);
                break;
            }
            case GeometryType::square: {
                Square* square = new Square();
                square = dynamic_cast<Square*>(square->deserialize(item));
                scene.items.push_back(square);
                break;
            }
            case GeometryType::mesh: {
                Mesh* mesh = new Mesh();
                mesh = dynamic_cast<Mesh*>(mesh->deserialize(item));
                scene.items.push_back(mesh);
                break;
            }
            case GeometryType::triangle: {
                Triangle* triangle = new Triangle();
                triangle = dynamic_cast<Triangle*>(triangle->deserialize(item));
                scene.items.push_back(triangle);
                break;
            }/*
            case GeometryType::bvh: {
                BVH* bvh = new BVH();
                bvh = dynamic_cast<BVH*>(bvh->deserialize(item));
                scene.items.push_back(bvh);
                break;
            }*/
            default:
                break;
        }
    }

    for(auto light_ : json_["lights"]) {
        int type_ = light_["type"];
        LightType type = (LightType)type_;
        switch(type) {
            case LightType::rectangle:{
                RectangleLight* light = new RectangleLight();
                light = dynamic_cast<RectangleLight*>(light->deserialize(light_));
                scene.lights.push_back(light);
                break;
            }
            default:
                break;
        }
    }
    return scene;
}