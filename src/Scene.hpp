#include <iostream>
#include "Quad.hpp"
#include "./ModelLoader.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene(std::string in_rootDir);

    void render() const;
private:
    Quad *background;
	ModelLoader *test_bg;
	ModelLoader *test_star;
    std::string rootDir = "";
    
};

#endif // SCENE_H
