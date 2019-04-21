#include <iostream>
#include "Quad.hpp"
#include "./ModelLoader.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene();

    void render() const;


private:
    Quad *background;
	ModelLoader *test_bg;
	ModelLoader *test_star;
    
};

#endif // SCENE_H
