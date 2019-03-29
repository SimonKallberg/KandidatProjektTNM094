//#include "Player.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
    // Constructor
    DomeGame(sgct::Engine * gEngine);
    
    void init();
    void draw() const;
    size_t textureHandle;
    Scene * myScene;
private:
    
};

#endif // DOMEGAME
