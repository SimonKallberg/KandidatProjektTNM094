#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <math.h>
#include "./Quad.hpp"
#include "Bullet.hpp"

class Player {

public:
    
    Player(std::string &name, std::vector<Bullet*> * inBulletList);

    Player(float in_theta, float in_phi, float in_dir, std::string &name, std::vector<Bullet*> * inBulletList);
    
    ~Player();
    
	// used for spawning maybe
    void setPosition(float x, float y);
    
    void shoot();

	// moving the player, prob collision testing etc as well
	void update();
    
    float getTheta();
    
    float getPhi();
    
    void render() const;

	// controls, 1 for pressed, 0 if it's not
	int c_left = 0;
	int c_right = 0;
    
private:
	// player avatar image(sprite is a word for an in game image)
	Quad *sprite = new Quad("player", 0.2f, 0.2f);

	const float RADIUS = 7.4f;

	float speed = 0.1f;
	float turn_speed = 0.05f;

    //Spawns new players at origin
    float theta = 0;
    float phi = 0;

	// velocities
	float v_theta = 0;
	float v_phi = 0;

    float direction = 0; //angle(degrees)  0 = straight up, 90 = right

	std::string playerName;
    
    //bullet list
    std::vector<Bullet*> * bullets = nullptr;
};

#endif // PLAYER

