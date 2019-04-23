//#include <iostream>
//#include <math.h>

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include "./Quad.hpp"
#include "./Bullet.hpp"

class Bullet;


class Player {

public:
    
    Player(std::string &name, std::vector<Bullet*> * inBulletList);

    Player(float in_theta, float in_phi, float in_dir, std::string &name, std::vector<Bullet*> * inBulletList);
    
    ~Player();
    
	// used for spawning maybe
    void setPosition(float x, float y);
    
    void shoot();

	// moving the player, prob collision testing etc as well
	void update(float dt);

	glm::quat getQuat();
	glm::mat4 getRotationMatrix();
	glm::vec3 getWorldVelocity(); // still in rotational units, but aligned with the XYZ world

	// apply a velocity from the xyz world to the local velocity. Orthogonal part to dome is ignored
	void addWorldVelocity(glm::vec3 w_vel);

	// apply a translation from the xyz world to the local velocity. Orthogonal part to dome is ignored. The parallel part is in rotation units(rads)
	void addWorldTranslation(glm::vec3 w_trans);

    void decreaseScore();
    
    void increaseScore();
    
    void render() const;

	// controls, 1 for pressed, 0 if it's not
	int c_left = 0;
	int c_right = 0;
	int c_shoot = 0;

	static void initSprite();

	static void bindSprite();
    
private:
	// player avatar image(sprite is a word for an in game image)
	// this static quad is drawn multiple times in a frame, 1 per player
	static Quad * sprite;
	float acceleration = 0.2f; // unit/s^2
	float turn_speed = 3.0f; // radians/s
	float slowdown = 0.6; // speed multiplier per second, if no acceleration then after one second velocity = velocity * slowdown;

	//Spawns new players at origin
	glm::quat position = glm::quat(1,0,0,0);

	float direction = 0; //angle(radians)  0 = straight up, 90 = right

	// velocities
	float up_vel = 0.0f;
	float right_vel = 0.0f;


	int score = 0;
	std::string playerName;
    
    //bullet list
    std::vector<Bullet*> * bullets = nullptr;
	float lastShotTime = 0.0f; //how long ago the last bullet was fired
};

#endif // PLAYER

