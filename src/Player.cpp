#include "./Player.hpp"

std::string Player::getName() {
	return playerName;
}

Weapon * Player::getWeapon() const {
	return weapon;
}

void Player::setWeapon(Weapon* wp, std::string type) {
	if (weapon)
		delete weapon;
	weapon = wp;
	weaponType = type;
}

void Player::update(float dt) {
	direction += dt * turn_speed * (c_right - c_left);

	if (alive) {
		up_vel += dt * acceleration * cos(direction);
		right_vel += dt * acceleration * sin(direction);

		if (health < 100)
			health += dt * 10;
	} else {
		up_vel += dt * acceleration * cos(direction) * deathSpeedFactor;
		right_vel += dt * acceleration * sin(direction) * deathSpeedFactor;

		deathtimer += dt;

		if (deathtimer > 5.0f) {
			alive = true;
			health = 100;
		}
	}

	flashtimer += dt;
		
	DomeMovable::update(dt);
}


void Player::applyWeaponKnockback(float knock) {
	up_vel -= knock * cos(direction);
	right_vel -= knock * sin(direction);
}


void Player::takeDamage(float dmg){
	health -= dmg;

	if (health <= 0.0f) {
		alive = false;
		deathtimer = 0.0f;
		score -= 100;
	}
}

bool Player::isAlive() {
	return alive;
}

void Player::increaseScore(int points){
    score += points;
}


glm::vec4 Player::getAmbient() {
	glm::vec3 ambient = glm::vec3(0.6f);
	float alpha;
	if (alive) {
		alpha = 1;
		float redadd = (100 - health) / 100.0f * ( 0.6f + 0.4 * sin(flashtimer * (100 ) / 10.0f) );
		ambient += glm::vec3(redadd, -redadd, -redadd);
	} else {
		alpha = 0.6f + 0.2f * cos(flashtimer * 20.0f);
	}
		
	return glm::vec4(ambient, alpha);
}


void Player::writeData() {
	sgct::SharedFloat s_flash = flashtimer;
	sgct::SharedData::instance()->writeFloat(&s_flash);

	sgct::SharedFloat s_health = health;
	sgct::SharedData::instance()->writeFloat(&s_health);

	sgct::SharedBool s_alive = alive;
	sgct::SharedData::instance()->writeBool(&s_alive);

	DomeDrawable::writeData();
}

void Player::readData() {
	sgct::SharedFloat s_flash;
	sgct::SharedData::instance()->readFloat(&s_flash);
	flashtimer = s_flash.getVal();

	sgct::SharedFloat s_health;
	sgct::SharedData::instance()->readFloat(&s_health);
	health = s_health.getVal();

	sgct::SharedBool s_alive;
	sgct::SharedData::instance()->readBool(&s_alive);
	alive = s_alive.getVal();

	DomeDrawable::readData();
}


