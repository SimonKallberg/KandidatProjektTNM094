#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <math.h>
#include "./Quad.hpp"

class Player {

public:
    
    Player();
    
    Player(int in_theta, int in_phi, int in_dir);
    
    ~Player();
    
    void setPosition(float x, float y);
    
    float getTheta();
    
    float getPhi();
    
    void render() const;
    
private:
    //Spawns new players at origin
    float theta = 0;
    float phi = 0;
    //float score;
    float direction = 0; //angle unit cirle
    Quad *texture = new Quad("player", 0.1f, 0.1f);
    const float RADIUS = 7.4f;
    
};

#endif // PLAYER

