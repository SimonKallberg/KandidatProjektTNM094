#ifndef Projectile_hpp
#define Projectile_hpp

#include "./DomeMovable.hpp"
#include "./Player.hpp"

class Projectile : public DomeMovable {
    
public:
    
    //Constructors
	Projectile() = delete;
	Projectile(std::string &tex, glm::quat pos, Player * in_owner)
		: DomeMovable(tex, pos), owner(in_owner) {}

	
	void init(); // Sets velocites and spreads
	void collide();
	bool alive() const; // For collision test
	glm::vec4 getColor() const;
	glm::vec3 getLightColor() const;
    
    //Virtual functions
    virtual bool update(float dt);
	
    //Public member variables
    Player* getOwner();
	float damage = 1;
    
protected:
	
	// Propulsion from the player weapon.
	float propulsionSpeed = 1.0f;

	// Random spread
	float propulsionSpread = 0.0f;
	float directionSpread = 0.0f;
	float sizeSpread = 0.0f;

    // Lifetime
	float lifetimeLeft; // seconds left to deletion
	float lifetime = 1.0f; // total seconds to live

    // Deathtime
	float deathtime = 1.0f;
	float deathtimeLeft = 1.0f;

    //Colors
	glm::vec3 color = glm::vec3(1, 1, 1);
	glm::vec3 deathColor = glm::vec3(1, 1, 1);
	float alpha = 1;

    //Player who fired the projectile
    Player *owner = nullptr;
};

class ShotgunPellet : public Projectile {
public:
    //Constructors
	ShotgunPellet() = delete;
	ShotgunPellet(glm::quat pos, Player * in_owner);
};

class SMGRound : public Projectile {
public:
    //Constructors
	SMGRound() = delete;
	SMGRound(glm::quat pos, Player * in_owner);
};

class LightBall : public Projectile {
public:
    //constructors
	LightBall() = delete;
	LightBall(glm::quat pos, Player * in_owner);
};
#endif 
