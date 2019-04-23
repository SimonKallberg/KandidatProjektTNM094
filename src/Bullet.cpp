#include "./Bullet.hpp"

Bullet::Bullet(glm::quat pos, float dir, float up_v, float right_v, Player * in_owner)
	: position(pos), direction(dir), up_vel(up_v), right_vel(right_v)
{
	// size spread
	size += ((float)rand() / RAND_MAX) * sizeSpread - sizeSpread / 2;

	//offset
	position *= glm::quat(0.1f * glm::vec3(cos(dir), 0.0f, 0.0f));
	position *= glm::quat(0.1f * glm::vec3(0.0f, sin(-dir), 0.0f));

	//spread
	direction += ((float)rand() / RAND_MAX) * spread - spread / 2;

	//add weapon propulsion
	up_vel += speed * cos(direction);
	right_vel += speed * sin(direction);

    owner = in_owner;
}

bool Bullet::update(float dt){
	up_vel *= pow(slowdown, dt);
	right_vel *= pow(slowdown, dt);

	position *= glm::quat(dt * glm::vec3(up_vel, 0.0f, 0.0f));
	position *= glm::quat(dt * glm::vec3(0.0f, -right_vel, 0.0f));
	position = glm::normalize(position);

    lifeTime -= dt;
    if(lifeTime < 0)
    {
        return false;
    }
    else return true;
}

void Bullet::render(){
	sprite->bindVAO();
	sprite->setTexture("bullet");
	sprite->render();
    
}


Player* Bullet::getOwner()
{
    return owner;
}

glm::quat Bullet::getQuat() {
	return position;
}

glm::mat4 Bullet::getRotationMatrix() {
	return glm::toMat4(position * glm::quat(glm::vec3(0, 0, -direction)));
}

glm::vec3 Bullet::getWorldVelocity() {
	return position * glm::vec3(right_vel, up_vel, 0);
}

float Bullet::getSize() {
	return size;
}


Quad* Bullet::sprite = nullptr;
void Bullet::initSprite() {
	sprite = new Quad("player", 1.0f);
}

void Bullet::bindSprite() {
	sprite->bindVAO();
}
