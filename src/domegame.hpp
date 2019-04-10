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
    void render() const;
    void addPlayer(std::string &name);
    void addPlayer(std::string &name, float in_theta, float in_phi, float in_dir);
    void update();
    
    size_t textureHandle;
    Scene * myScene;
    std::vector<Player*> players;
    std::vector<Bullet*> bullets;
    
private:
    
};

#endif // DOMEGAME
