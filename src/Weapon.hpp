#ifndef Weapon_hpp
#define Weapon_hpp

#include "./DomeDrawable.hpp"
#include "./Projectile.hpp"
#include "./Player.hpp"

class Weapon : public DomeDrawable{
public:
	Weapon() = delete;
	Weapon(std::string &tex, Player* in_owner)
		: DomeDrawable(tex, glm::quat()), owner(in_owner) {}

	glm::mat4 getRotationMatrix();

	virtual void shoot() = 0;

	virtual void update(float dt, int c_shoot);

	// used to set the projectile list pointer
	static void init(std::vector<Projectile*> *list);

protected:
	static std::vector<Projectile*> *projectiles;
	Player* owner;

	float chargedTime = 0.0f; // how long the fire button has been held down
	float chargeTime = 0.0f; // how long it needs to be held down to shoot
	
	float reloadTimeLeft = 0.0f; // time left until weapon can shoot again
	float reloadTime = 1.0f; // reload time between shots

	// pos quat is used as the weapon offset relative to player when rendering
	float weaponOffsetUp = 0.0f;
	float weaponOffsetRight = 0.0f;

	float projectileOffsetUp = 0.0f;
	float projectileOffsetRight = 0.0f;

	glm::quat projectileOffset;

	float weaponBloatSize = 0.0f;
	float weaponSize = 1.0f;

	float visualRecoil = 0.0f;
	float visualRecoilRecovery = 0.0f;
	float currentRecoil = 0.0f;
	
};

class Shotgun : public Weapon{
public:
	Shotgun() = delete;
	Shotgun(Player* in_owner);
	void update(float dt, int c_shoot);
	void shoot();
protected:
	int pellets;
};

#endif