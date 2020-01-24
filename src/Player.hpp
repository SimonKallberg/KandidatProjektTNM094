#ifndef Player_hpp
#define Player_hpp

#include "./DomeMovable.hpp"

class Weapon;

class Player : public DomeMovable {

public:
    
	Player() = delete;
	Player(std::string &name, std::string &tex, glm::quat pos = glm::quat())
	: playerName(name), DomeMovable(tex, pos)
	{
		weapon = nullptr;
		slowdown = 0.3f;
		scale = 0.15f;

		turn_speed = 3.0f;
		acceleration = scale;

		bumpTexture = "playerbump";
	};

	std::string weaponType;
	std::string getName();

	Weapon * getWeapon() const;
	void setWeapon(Weapon* wp, std::string type);

	glm::vec4 getAmbient();
    
	void update(float dt);

	void applyWeaponKnockback(float knock);

	bool isAlive();
    void takeDamage(float dmg);
    
    void increaseScore(int points);

	// controls, 1 for pressed, 0 if it's not
	int c_left = 0;
	int c_right = 0;
	int c_shoot = 0;
    
    int score = 0;
    std::string playerName;

	void writeData();
	void readData();

private:
	// health and a bool to quicky check if it's below 0
	float health = 100;
	bool alive = true;

	// used for flashing effects (basically a gameclock but unique to the player)
	float flashtimer = 0;

	// timer used to return to life after death
	float deathtimer = 0;

	Weapon* weapon;
	
	float acceleration = 0.0f; // unit/s^2
	float turn_speed = 0.0f; // radians/s

	float deathSpeedFactor = 2.0f;
   
};

#endif // PLAYER

