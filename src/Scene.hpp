#include <iostream>
#include "Quad.hpp"
#include "./ModelLoader.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene();

    void draw() const;
private:
    Quad *background;
	ModelLoader *test_bg;
    
};

#endif // SCENE_H
