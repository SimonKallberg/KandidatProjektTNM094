#ifndef DOMEGAME
#define DOMEGAME

#include "./Player.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>


class DomeGame{
public:
    // Constructor
    DomeGame(sgct::Engine * gEngine);
    
    void init();
    void render() const;
    void addPlayer();
	void update();
    
    size_t textureHandle;
    Scene * myScene;
    std::vector<Player*> players;
    std::vector<Bullet*> bullets;
    
private:
    
};

#endif // DOMEGAME
