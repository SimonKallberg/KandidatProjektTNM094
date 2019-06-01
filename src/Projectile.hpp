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

	//Public member functions
	void init(); // Sets velocites and spreads
	void collide();
	bool alive() const; // For collision test
	glm::vec4 getColor() const;
	glm::vec3 getLightColor() const;
    
    //Virtual functions
    virtual bool update(float dt);
	
    //Public member variables
    Player* getOwner();

	float damage = 0;
	float knockback = 0;

	void writeData();
	void readData();
    
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
	float hitDeathTime = 0.0f;
	float missDeathTime = 0.0f;
	float deathtimeLeft = 0.0f;

    //Colors
	glm::vec3 color = glm::vec3(1, 1, 1);
	glm::vec3 hitColor = glm::vec3(1, 1, 1);
	glm::vec3 lightColor = glm::vec3(1, 1, 1);
	float alpha = 1;

    //Player who fired the projectile
    Player *owner = nullptr;
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




class PopBall : public Projectile {
public:
	//constructors
	PopBall() = delete;
	PopBall(glm::quat pos, Player * in_owner);
};

class ShotgunPellet : public Projectile {
public:
	//Constructors
	ShotgunPellet() = delete;
	ShotgunPellet(glm::quat pos, Player * in_owner, float diradd);
};

class RifleBullet : public Projectile {
public:
	//Constructors
	RifleBullet() = delete;
	RifleBullet(glm::quat pos, Player * in_owner);
};
#endif 
