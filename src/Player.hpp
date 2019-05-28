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
		slowdown = 0.4f;
		scale = 0.4f;
		bumpTexture = "playerbump";
	};

	std::string weaponType;
	std::string getName();

	Weapon * getWeapon() const;
	void setWeapon(Weapon* wp, std::string type);
    
	void update(float dt);

    void decreaseScore();
    
    void increaseScore();

	// controls, 1 for pressed, 0 if it's not
	int c_left = 0;
	int c_right = 0;
	int c_shoot = 0;

private:

	Weapon* weapon;
	
	float acceleration = 0.2f; // unit/s^2
	float turn_speed = 3.0f; // radians/s

	int score = 0;
	std::string playerName;
   
};

#endif // PLAYER

