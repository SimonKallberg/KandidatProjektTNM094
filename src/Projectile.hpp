#ifndef Projectile_hpp
#define Projectile_hpp

#include "./DomeMovable.hpp"
#include "./Player.hpp"

class Projectile : public DomeMovable {
    
public:
	Projectile() = delete;
	Projectile(std::string &tex, glm::quat pos, Player * in_owner)
		: DomeMovable(tex, pos), owner(in_owner) {}

	// set velocites and spreads
	void init();
    
    virtual bool update(float dt);

	void collide();

	bool alive() const; // for collision test

	glm::vec4 getColor() const;
	glm::vec3 getLightColor() const;
	
    Player* getOwner();
	float damage = 1;

protected:
	
	// propulsion from the player weapon.
	float propulsionSpeed = 1.0f;

	// random spread
	float propulsionSpread = 0.0f;
	float directionSpread = 0.0f;
	float sizeSpread = 0.0f;

	float lifetimeLeft; // seconds left to deletion
	float lifetime = 1.0f; // total seconds to live

	float deathtime = 1.0f;
	float deathtimeLeft = 1.0f;

	glm::vec3 color = glm::vec3(1, 1, 1);
	glm::vec3 deathColor = glm::vec3(1, 1, 1);

	float alpha = 1;


    Player *owner = nullptr;
};

class ShotgunPellet : public Projectile {
public:
	ShotgunPellet() = delete;
	ShotgunPellet(glm::quat pos, Player * in_owner);
};

class SMGRound : public Projectile {
public:
	SMGRound() = delete;
	SMGRound(glm::quat pos, Player * in_owner);
};

class LightBall : public Projectile {
public:
	LightBall() = delete;
	LightBall(glm::quat pos, Player * in_owner);
};
#endif 
