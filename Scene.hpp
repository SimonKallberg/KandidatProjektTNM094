#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>
#include <map>

#include "sgct.h"
#include "Quad.hpp"

class Player;

class Scene {
public:
    // Constructor
    Scene();

    void draw();
private:
    Quad *background;
    
};

#endif // SCENE_H
