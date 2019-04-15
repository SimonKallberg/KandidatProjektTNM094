#include "./Quad.hpp"
#include "./Player.hpp"

#ifndef Bullet_hpp
#define Bullet_hpp

class Player;

class Bullet {
    
public:
    Bullet() = delete;
    Bullet(float in_phi, float in_theta, float in_direction, Player* in_owner);
    ~Bullet();
    
    bool update();
    void render();
    float getPhi();
    float getTheta();
    Player* getOwner();
    

    
private:
    Quad *sprite = new Quad("bullet", 0.1f, 0.1f);
    const float RADIUS = 7.4f;
    float lifeTime = 50.0f;
    float phi = 0.0f;
    float theta = 0.0f;
    float direction = 0.0f;
    float speed = 0.2f;
    Player *owner = nullptr;
};

#endif 
