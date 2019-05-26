#include"./Weapon.hpp"

glm::mat4 Weapon::getRotationMatrix() {
	return glm::toMat4(glm::quat(glm::vec3(-currentRecoil, 0, 0)) * getQuat());
}

void Weapon::update(float dt, int c_shoot) {
	reloadTimeLeft -= dt;
	chargedTime = c_shoot * (chargedTime + dt);

	if (visualRecoilRecovery > 0.0001f)
		currentRecoil = max(currentRecoil - dt * visualRecoil / visualRecoilRecovery, 0.0f);

	if (reloadTimeLeft < 0.0f) {
		if (chargeTime > 0.0001f)
			scale = weaponSize + weaponBloatSize * chargedTime / chargeTime;

		if (chargedTime > chargeTime) {
			shoot();
			reloadTimeLeft = reloadTime;
			scale = weaponSize;
			currentRecoil = visualRecoil;
		}
	}
}

std::vector<Projectile>* Weapon::projectiles = nullptr;
sgct::SharedVector<Projectile>* Weapon::added_projectiles = nullptr;
void Weapon::init(std::vector<Projectile> *list, sgct::SharedVector<Projectile> *add_list) {
	projectiles = list;
	added_projectiles = add_list;
}



void Weapon::writeData() {
	DomeDrawable::writeData();

	sgct::SharedFloat s_rec = currentRecoil;
	sgct::SharedData::instance()->writeFloat(&s_rec);
}

void Weapon::readData() {
	DomeDrawable::readData();

	sgct::SharedFloat s_rec;
	sgct::SharedData::instance()->readFloat(&s_rec);
	currentRecoil = s_rec.getVal();
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
	for (int i = 0; i < pellets; i++) {
		projectiles->push_back(ShotgunPellet(p_pos, owner));
		added_projectiles->addVal(ShotgunPellet(p_pos, owner));
	}
}

void Shotgun::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}



//smg

SMG::SMG(Player * in_owner)
	: Weapon(std::string(), in_owner)
{
	texture = "venus";
	weaponSize = 0.4f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.02f;
	visualRecoilRecovery = 0.01f;

	weaponOffsetUp = 0.1f;
	weaponOffsetRight = 0.0f;
	projectileOffsetUp = 0.15f;
	projectileOffsetRight = 0.0f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 0.02f;
	chargeTime = 0.5f;
}

void SMG::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset; std::cout << owner->getQuat().x << " ,  " << owner->getQuat().y << " ,  " << owner->getQuat().z << " ,  " << "\n";
	projectiles->push_back(SMGRound(p_pos, owner));
	added_projectiles->addVal(SMGRound(p_pos, owner));
}

void SMG::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}


// LightBallLauncher

LightBallLauncher::LightBallLauncher(Player * in_owner)
	: Weapon(std::string(), in_owner)
{
	texture = "weapon1";
	bumpTexture = "weapon1normal";
	weaponSize = 0.8f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.0f;
	visualRecoilRecovery = 0.0f;

	weaponOffsetUp = 0.00f;
	weaponOffsetRight = 0.08f;
	projectileOffsetUp = 0.06f;
	projectileOffsetRight = 0.08f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 1.0f;
	chargeTime = 0.0f;
}

void LightBallLauncher::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset; std::cout << owner->getQuat().x << " ,  " << owner->getQuat().y << " ,  " << owner->getQuat().z << " ,  " << "\n";
	projectiles->push_back(LightBall(p_pos, owner));
	added_projectiles->addVal(LightBall(p_pos, owner));
}


void LightBallLauncher::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}

