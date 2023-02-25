#include "scene_loader.hpp"

void SceneLoader::createSceneFile(json json_) {
    std::string name = json_["name"];
    std::string path = "../data/"+name;
    std::experimental::filesystem::create_directory(path);
    std::experimental::filesystem::create_directory(path+"/renders");

    writeJson(json_);
}

void SceneLoader::writeJson(json json_) {
    std::string name = json_["name"];
    std::string path = "../data/"+name+"/"+name+".json";

    std::ofstream output(path);
    output << json_.dump(4);
}

json SceneLoader::readJson(std::string name) {
    std::string path = "../data/"+name+"/"+name+".json";
    std::ifstream input(path);

    json json_;
    input >> json_;
    return json_;
}