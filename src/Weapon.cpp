#include"./Weapon.hpp"
//Initializing variables
std::string empty_string1 = "";
std::vector<Projectile>* Weapon::projectiles = nullptr;
sgct::SharedVector<Projectile>* Weapon::added_projectiles = nullptr;

glm::mat4 Weapon::getRotationMatrix() {
	return glm::toMat4(glm::quat(glm::vec3(-currentRecoil, 0, 0)) * getQuat());
}

//Static function
void Weapon::init(std::vector<Projectile> *list, sgct::SharedVector<Projectile> *add_list) {
	projectiles = list;
	added_projectiles = add_list;
}

Weapon* Weapon::makeWeapon(std::string wType, Player* own) {
	if (wType == "shotgun")
		return new Shotgun(own);
	if (wType == "popgun")
		return new PopGuns(own);
	if (wType == "rifle")
		return new Rifle(own);

	// testweapons
	if (wType == "smg")
		return new SMG(own);
	if (wType == "light")
		return new LightBallLauncher(own);
}




void Weapon::update(float dt, int c_shoot) {
	reloadTimeLeft -= dt;

	if(owner->isAlive()){
		
		if (visualRecoilRecovery > 0.0001f)
			currentRecoil = fmax(currentRecoil - dt * visualRecoil / visualRecoilRecovery, 0.0f);

		scale = weaponSize + weaponBloatSize * (1 - fmax(reloadTimeLeft / reloadTime,0));

		if (reloadTimeLeft < 0.0f && c_shoot == 1) {
			shoot();
			reloadTimeLeft = reloadTime;
			scale = weaponSize;
			currentRecoil = visualRecoil;
		}
	} else {
		currentRecoil = 0;
		scale = weaponSize;
	}
}



//smg

SMG::SMG(Player * in_owner)
	: Weapon(empty_string1, in_owner)
{
	texture = "venus";
	weaponSize = 0.4f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.02f;
	visualRecoilRecovery = 0.01f;

	float weaponOffsetUp = 0.1f;
	float weaponOffsetRight = 0.0f;
	float projectileOffsetUp = 0.15f;
	float projectileOffsetRight = 0.0f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 0.02f;
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
	: Weapon(empty_string1, in_owner)
{
	texture = "weapon1";
	bumpTexture = "weapon1normal";
	weaponSize = 0.8f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.0f;
	visualRecoilRecovery = 0.0f;

	float weaponOffsetUp = 0.00f;
	float weaponOffsetRight = 0.08f;
	float projectileOffsetUp = 0.06f;
	float projectileOffsetRight = 0.08f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 1.0f;
}

void LightBallLauncher::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset;
	projectiles->push_back(LightBall(p_pos, owner));
	added_projectiles->addVal(LightBall(p_pos, owner));
}


void LightBallLauncher::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}


void Weapon::writeData() {
	sgct::SharedFloat s_recoil = currentRecoil;
	sgct::SharedData::instance()->writeFloat(&s_recoil);

	DomeDrawable::writeData();
}

void Weapon::readData() {
	sgct::SharedFloat s_recoil;
	sgct::SharedData::instance()->readFloat(&s_recoil);

	currentRecoil = s_recoil.getVal();

	DomeDrawable::readData();
}








// real weapons


// Pop guns

PopGuns::PopGuns(Player * in_owner)
	: Weapon(empty_string1, in_owner)
{
	doubleWeapon = true;

	texture = "weapon2_right";
	leftWeaponTexture = "weapon2_left";
	rightWeaponTexture = "weapon2_right";

	bumpTexture = "weapon2normal_right";
	leftWeaponBumpTexture = "weapon2normal_left";
	rightWeaponBumpTexture = "weapon2normal_right";

	weaponSize = 1.5f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.0f;
	visualRecoilRecovery = 0.0f;

	float weaponOffsetUp = 0.01f;
	float weaponOffsetRight = 0.08f;
	float projectileOffsetUp = 0.03f;
	float projectileOffsetRight = 0.11f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	leftPosition *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	leftPosition *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, 1.0f, 0.0f));

	leftProjectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	leftProjectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, 1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 0.2f;
}

void PopGuns::shoot() {
	glm::quat p_pos;
	if(shootRight)
		p_pos = owner->getQuat() * projectileOffset;
	else
		p_pos = owner->getQuat() * leftProjectileOffset;

	projectiles->push_back(PopBall(p_pos, owner));
	added_projectiles->addVal(PopBall(p_pos, owner));

	shootRight = !shootRight;
}


void PopGuns::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}

glm::mat4 PopGuns::getSecondWeaponRotationMatrix() {
	return glm::toMat4(glm::quat(glm::vec3(-currentRecoil, 0, 0)) * leftPosition);
}

void PopGuns::switchWeaponTexture() {
	if (texture == leftWeaponTexture) {
		texture = rightWeaponTexture;
		bumpTexture = rightWeaponBumpTexture;
	} else {
		texture = leftWeaponTexture;
		bumpTexture = leftWeaponBumpTexture;
	}
		
}



//shotgun
Shotgun::Shotgun(Player * in_owner)
	: Weapon(empty_string1, in_owner)
{
	texture = "weapon1";
	bumpTexture = "weapon1normal";
	weaponSize = 0.8f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.01f;
	visualRecoilRecovery = 0.1f;

	float weaponOffsetUp = 0.00f;
	float weaponOffsetRight = 0.08f;
	float projectileOffsetUp = 0.06f;
	float projectileOffsetRight = 0.08f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 1.0f;
}

void Shotgun::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset;
	for (int i = -2; i <= 2; i++) {
		projectiles->push_back(ShotgunPellet(p_pos, owner, i*0.2f));
		added_projectiles->addVal(ShotgunPellet(p_pos, owner, i*0.2f));
	}
}

void Shotgun::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}



// rifle
Rifle::Rifle(Player * in_owner)
	: Weapon(empty_string1, in_owner)
{
	texture = "weapon3";
	bumpTexture = "weapon3normal";
	weaponSize = 1.0f;
	weaponBloatSize = 0.1f;

	visualRecoil = 0.01f;
	visualRecoilRecovery = 0.2f;

	float weaponOffsetUp = 0.14f;
	float weaponOffsetRight = 0.00f;
	float projectileOffsetUp = 0.18f;
	float projectileOffsetRight = 0.00f;

	//offset
	position *= glm::quat(owner->getScale() * weaponOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	position *= glm::quat(owner->getScale() * weaponOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetUp * glm::vec3(1.0f, 0.0f, 0.0f));
	projectileOffset *= glm::quat(owner->getScale() * projectileOffsetRight * glm::vec3(0.0f, -1.0f, 0.0f));

	scale = weaponSize; // relative to player

	reloadTime = 0.8f;
}

void Rifle::shoot() {
	glm::quat p_pos = owner->getQuat() * projectileOffset;
	projectiles->push_back(RifleBullet(p_pos, owner));
	added_projectiles->addVal(RifleBullet(p_pos, owner));
}


void Rifle::update(float dt, int c_shoot) {
	Weapon::update(dt, c_shoot);
}



