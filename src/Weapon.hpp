#ifndef Weapon_hpp
#define Weapon_hpp

#include "./DomeDrawable.hpp"
#include "./Projectile.hpp"
#include "./Player.hpp"

class Weapon : public DomeDrawable{
public:
    //Constructors
	Weapon() = delete;
	Weapon(std::string &tex, Player* in_owner)
		: DomeDrawable(tex, glm::quat()), owner(in_owner) {}

	static Weapon* makeWeapon(std::string wType, Player* own);

    //Public member functions
	void writeData();
	void readData();
    glm::mat4 getRotationMatrix();

	//for Double-Weapons
	virtual glm::mat4 getSecondWeaponRotationMatrix() { return glm::mat4(); };
	virtual void switchWeaponTexture() { };

	//Static functions
    // init: used to set the projectile list pointer
	static void init(std::vector<Projectile> *list,sgct::SharedVector<Projectile> *add_list);
    
    //Virtual functions
    virtual void shoot() = 0;
    virtual void update(float dt, int c_shoot);

	bool doubleWeapon = false;

protected:
    
    //Vectors with projectiles
	static std::vector<Projectile> *projectiles;
	static sgct::SharedVector<Projectile> *added_projectiles;

	Player* owner;

	std::string readyTexture;
	std::string coolDownTexture;

    //Reload time
	float reloadTimeLeft = 0.0f; // time left until weapon can shoot again
	float reloadTime = 1.0f; // reload time between shots

	glm::quat projectileOffset;

    //Weapon size
	float weaponBloatSize = 0.0f; // how much the weapon gets bigger when charging
	float weaponSize = 1.0f;

    //Recoil
	float visualRecoil = 0.0f; // how far the weapon rocks back a shot is fired
	float visualRecoilRecovery = reloadTime; // the time it takes to revert to default positon fromr recoil
	float currentRecoil = 0.0f;

	float selfKnockback = 0;
	
};

class Shotgun : public Weapon{
public:
    //Constructors
	Shotgun() = delete;
	Shotgun(Player* in_owner);
    
	void update(float dt, int c_shoot);
	void shoot();
};

class SMG : public Weapon {
public:
    //Constructors
	SMG() = delete;
	SMG(Player* in_owner);
    
	void update(float dt, int c_shoot);
	void shoot();
};

class LightBallLauncher : public Weapon {
public:
    //Constructors
	LightBallLauncher() = delete;
	LightBallLauncher(Player* in_owner);
    
	void update(float dt, int c_shoot);
	void shoot();
};

class PopGuns : public Weapon {
public:
	//Constructors
	PopGuns() = delete;
	PopGuns(Player* in_owner);

	void update(float dt, int c_shoot);
	void shoot();

	glm::mat4 getSecondWeaponRotationMatrix();
	void switchWeaponTexture();

	std::string leftReadyTexture;
	std::string leftCoolDownTexture;

	std::string leftWeaponTexture;
	std::string rightWeaponTexture;
	std::string leftWeaponBumpTexture;
	std::string rightWeaponBumpTexture;

	float leftRecoil = 0.0f;
	float rightRecoil = 0.0f;

	glm::quat leftPosition;
	glm::quat leftProjectileOffset;

	// bool to alternate which weapon shoots
	bool shootRight = true;
};

class Rifle : public Weapon {
public:
	//Constructors
	Rifle() = delete;
	Rifle(Player* in_owner);

	void update(float dt, int c_shoot);
	void shoot();
};


#endif
