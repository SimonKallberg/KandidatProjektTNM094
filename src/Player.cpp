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
	up_vel += dt * acceleration * cos(direction);
	right_vel += dt * acceleration * sin(direction);
	DomeMovable::update(dt);
}

void Player::decreaseScore(){
    score -= 1;
    std::cout << "The player" << playerName << " has score " <<  score << std::endl;
}

void Player::increaseScore(){
    score += 1;
    std::cout << "The player" << playerName << " has score " <<  score << std::endl;
}




