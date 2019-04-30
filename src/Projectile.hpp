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
	
    Player* getOwner();
        
protected:
	
	// propulsion from the player weapon.
	float propulsionSpeed = 1.0f;

	// random spread
	float propulsionSpread = 0.0f;
	float directionSpread = 0.0f;
	float sizeSpread = 0.0f;

	float timeLeft; // seconds left to deletion
	float lifeTime = 1.0f; // total seconds to live
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

#endif 
