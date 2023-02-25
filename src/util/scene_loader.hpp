#include "../scene.hpp"
#include <fstream>
#include <experimental/filesystem>

class SceneLoader {
public:
    void createSceneFile(json json_);
    void writeJson(json json_);
    json readJson(std::string name);
};