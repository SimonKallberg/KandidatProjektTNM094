#include "./Player.hpp"

 
Player::Player(std::string & name, std::vector<Bullet*> * inBulletList)
: playerName(name), bullets(inBulletList)
{
}

Player::Player(float theta, float phi, float in_dir, std::string& name, std::vector<Bullet*> * inBulletList)
: position(glm::vec3(phi,theta,0)), direction(in_dir), playerName(name), bullets(inBulletList)
{
}

Player::~Player(){
}

void Player::setPosition(float x, float y){
    direction = x+y;
}

void Player::shoot() {
    Bullet * newBullet = new Bullet(position, direction, up_vel, right_vel, this);
    bullets->push_back(newBullet);
}

void Player::update(float dt) {
	direction += dt * turn_speed * (c_right - c_left);
	up_vel += dt * acceleration * cos(direction);
	right_vel += dt * acceleration * sin(direction);
	up_vel *= pow(slowdown,dt);
	right_vel *= pow(slowdown, dt);
	//std::cout << up_vel << right_vel << std::endl;

	position *= glm::quat(dt * glm::vec3(up_vel, 0.0f, 0.0f));
	position *= glm::quat(dt * glm::vec3(0.0f, -right_vel, 0.0f));
	position = glm::normalize(position); // quat slowly changes abs-value due to rounding

	lastShotTime += dt;
	if (lastShotTime > 0.1f && c_shoot == 1) {
		shoot();
		lastShotTime = 0;
	}
	//std::cout << position.w <<", " << position.x << ", " << position.y << ", " << position.z << "\n";
	//std::cout << position.w*position.w + position.x*position.x + position.y*position.y + position.z*position.z << std::endl;
}

void Player::decreaseScore(){
    score -= 1;
    std::cout << "The player" << playerName << " has score " <<  score << std::endl;
}

void Player::increaseScore(){
    score += 1;
    std::cout << "The player" << playerName << " has score " <<  score << std::endl;
}


void Player::render() const{
	sprite->bindVAO();
	sprite->setTexture("player");
	sprite->render();
}

glm::quat Player::getQuat() {
	return position;
}

glm::mat4 Player::getRotationMatrix() {
	return glm::toMat4(position * glm::quat(glm::vec3(0,0,-direction)));
}

glm::vec3 Player::getWorldVelocity() {
	return position * glm::vec3(right_vel, up_vel, 0);
}

void Player::addWorldVelocity(glm::vec3 w_vel) {
	glm::vec3 local_vel = glm::inverse(position) * w_vel;
	up_vel += local_vel.y;
	right_vel += local_vel.x;
}

void Player::addWorldTranslation(glm::vec3 w_trans) {
	glm::vec3 local_trans = glm::inverse(position) * w_trans;
	position *= glm::quat(glm::vec3(local_trans.y, 0.0f, 0.0f));
	position *= glm::quat(glm::vec3(0.0f, -local_trans.x, 0.0f));
	position = glm::normalize(position);
}


Quad* Player::sprite = nullptr;
void Player::initSprite() {
	sprite = new Quad("player", 1.0f);
}

void Player::bindSprite() {
	sprite->bindVAO();
}

