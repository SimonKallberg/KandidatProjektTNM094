#ifndef Bullet_hpp
#define Bullet_hpp

#include "./Quad.hpp"

class Bullet {
    
public:
    Bullet() = delete;
    Bullet(float in_phi, float in_theta, float in_direction);
    ~Bullet();
    
    bool update();
    void render();
    

    
private:
    Quad *sprite = new Quad("bullet", 0.1f, 0.1f);
    const float RADIUS = 7.4f;
    float lifeTime = 50.0f;
    float phi = 0.0f;
    float theta = 0.0f;
    float direction = 0.0f;
    float speed = 0.2f;
};

#endif 
