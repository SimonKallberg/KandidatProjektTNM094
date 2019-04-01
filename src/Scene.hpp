#include <iostream>
#include "Quad.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene();

    void draw() const;
private:
    Quad *background;
    
};

#endif // SCENE_H
