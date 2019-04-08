#include "./Player.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
    // Constructor
    DomeGame(sgct::Engine * gEngine);
    
    void init();
    void draw() const;
    void addPlayer(Player * newPlayer);
    void renderPlayer(Player * newPlayer);
    
    size_t textureHandle;
    Scene * myScene;
    std::vector<Player*> players;
    
private:
    
};

#endif // DOMEGAME
