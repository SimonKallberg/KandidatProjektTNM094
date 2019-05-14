#include "./Projectile.hpp"

void Projectile::init() {
	timeLeft = lifeTime;
	direction = 0;

	//proplsion spread
	propulsionSpeed += ((float)rand() / RAND_MAX - 0.5f) * propulsionSpread;
	//direction spread
	direction += ((float)rand() / RAND_MAX - 0.5f) * directionSpread;
	// size spread
	scale += ((float)rand() / RAND_MAX - 0.5f) * sizeSpread;

	//add weapon propulsion
	up_vel += propulsionSpeed * cos(direction);
	right_vel += propulsionSpeed * sin(direction);
}

bool Projectile::update(float dt){
	
	DomeMovable::update(dt);

    timeLeft -= dt;
    if(timeLeft < 0)
    {
        return false;
    }
    else return true;
}


Player* Projectile::getOwner()
{
    return owner;
}




//shotgun pellet

ShotgunPellet::ShotgunPellet(glm::quat pos, Player * in_owner)
	:  Projectile(std::string(), pos, in_owner)
{
	texture = "venus";
	lifeTime = 0.5f;
	scale = 0.1f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.05f;
	directionSpread = 0.6f;
	propulsionSpread = 0.5f;
	Projectile::init();
}


//smground

SMGRound::SMGRound(glm::quat pos, Player * in_owner)
	: Projectile(std::string(), pos, in_owner)
{
	texture = "player2";
	lifeTime = 0.5f;
	scale = 0.1f;
	propulsionSpeed = 1.5f;
	sizeSpread = 0.01f;
	directionSpread = 0.05f;
	propulsionSpread = 0.1f;
	Projectile::init();
}

LightBall::LightBall(glm::quat pos, Player * in_owner)
	: Projectile(std::string(), pos, in_owner)
{
	texture = "testbump";
	lifeTime = 60.0f;
	scale = 0.5f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.1f;
	Projectile::init();
}