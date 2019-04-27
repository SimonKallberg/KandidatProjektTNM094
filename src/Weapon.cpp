#include"./Weapon.hpp"

glm::mat4 Weapon::getRotationMatrix() {
	return glm::toMat4(glm::quat(glm::vec3(-currentRecoil, 0, 0)) * getQuat());
}

void Weapon::update(float dt, int c_shoot) {
	reloadTimeLeft -= dt;
	if (visualRecoilRecovery > 0.0001f)
		currentRecoil = max(currentRecoil - dt * visualRecoil / visualRecoilRecovery, 0.0f);

	if (reloadTimeLeft < 0.0f) {
		chargedTime = c_shoot * (chargedTime + dt);
		if (chargeTime > 0.0001f)
			scale = weaponSize + weaponBloatSize * chargedTime / chargeTime;

		if (chargedTime > chargeTime) {
			shoot();
			reloadTimeLeft = reloadTime;
			chargedTime = 0.0f;
			scale = weaponSize;
			currentRecoil = visualRecoil;
		}
	}
}

std::vector<Projectile*>* Weapon::projectiles = nullptr;
void Weapon::init(std::vector<Projectile*> *list) {
	projectiles = list;
}


//shotgun
Shotgun::Shotgun(Player * in_owner)
	: Weapon(std::string(), in_owner)
{
	texture = "bullet";
	weaponSize = 0.5f;
	weaponBloatSize = 0.2f;

	visualRecoil = 0.05f;
	visualRecoilRecovery = 1.0f;

	weaponOffsetUp = 0.05f;
	weaponOffsetRight = 0.05f;
	projectileOffsetUp = 0.1f;
	projectileOffsetRight = 0.05f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 0.5f;
	chargeTime = 0.5f;

	pellets = 20;
}

void Shotgun::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset;
	for(int i = 0; i < pellets; i++)
		projectiles->push_back(new ShotgunPellet(p_pos, owner));
}

void Shotgun::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}