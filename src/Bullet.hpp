#include "./Quad.hpp"
#include "./Player.hpp"

#ifndef Bullet_hpp
#define Bullet_hpp

class Player;

class Bullet  {
    
public:
    Bullet() = delete;
    Bullet(glm::quat pos, float dir, float up_v, float right_v, Player * in_owner);
    ~Bullet();
    
    bool update(float dt);
    void render();
	static void initSprite();
	static void bindSprite();
	glm::quat getQuat();
	glm::mat4 getRotationMatrix();
	glm::vec3 getWorldVelocity();
	float getSize();
    Player* getOwner();

        
private:
	static Quad *sprite;
    const float RADIUS = 7.4f;
	const float speed = 1.0f; // propulsion from the player weapon.
	float slowdown = 0.5; // speed multiplier per second, if no acceleration then after one second velocity = velocity * slowdown;
	float size = 0.5f;
	float spread = 0.5f;
	float sizeSpread = 0.8f;
	float direction;
	glm::quat position;
	float up_vel;
	float right_vel;

	float lifeTime = 2.0f; // seconds
    Player *owner = nullptr;
};

#endif 
