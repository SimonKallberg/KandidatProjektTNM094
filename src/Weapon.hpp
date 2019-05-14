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

	void writeData();
	void readData();

	// used to set the projectile list pointer
	static void init(std::vector<Projectile> *list,sgct::SharedVector<Projectile> *add_list);

protected:
	static std::vector<Projectile> *projectiles;
	static sgct::SharedVector<Projectile> *added_projectiles;

	Player* owner;

	float chargedTime = 0.0f; // how long the fire button has been held down
	float chargeTime = 0.0f; // how long it needs to be held down to start shooting
	
	float reloadTimeLeft = 0.0f; // time left until weapon can shoot again
	float reloadTime = 1.0f; // reload time between shots

	// pos quat is used as the weapon offset relative to player when rendering
	float weaponOffsetUp = 0.0f;
	float weaponOffsetRight = 0.0f;

	float projectileOffsetUp = 0.0f;
	float projectileOffsetRight = 0.0f;

	glm::quat projectileOffset;

	float weaponBloatSize = 0.0f; // how much the weapon gets bigger when charging
	float weaponSize = 1.0f;

	float visualRecoil = 0.0f; // how far the weapon rocks back a shot is fired
	float visualRecoilRecovery = 0.0f; // the time it takes to revert to default positon fromr recoil
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

class SMG : public Weapon {
public:
	SMG() = delete;
	SMG(Player* in_owner);
	void update(float dt, int c_shoot);
	void shoot();
protected:
};

class LightBallLauncher : public Weapon {
public:
	LightBallLauncher() = delete;
	LightBallLauncher(Player* in_owner);
	void update(float dt, int c_shoot);
	void shoot();
};

#endif