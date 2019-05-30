#include "./Projectile.hpp"

void Projectile::init() {
	lifetimeLeft = lifetime;
	deathtimeLeft = deathtime;
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

void Projectile::collide() {
	up_vel = 0;
	right_vel = 0;
	scale *= 1.2f;
	lifetimeLeft = -0.1f;
	color = deathColor;
}

bool Projectile::update(float dt){
	
	
    if(lifetimeLeft <= 0)
    {
        if(deathtimeLeft <= 0)
			return false;
		
		deathtimeLeft -= dt;

		float smallifier = pow(0.2, dt);;
		alpha *= smallifier;
		scale *= smallifier;
		slowdown = 0.1f;
    }

	DomeMovable::update(dt);
	lifetimeLeft -= dt;
	direction += 0.0f;

    return true;
}

bool Projectile::alive() const{
	if (lifetimeLeft > 0)
		return true;
	else
		return false;
}

glm::vec4 Projectile::getColor() const {
	return glm::vec4(color, alpha);
}
glm::vec3 Projectile::getLightColor() const {
	return color * alpha;
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
	lifetime = 0.5f;
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
	lifetime = 0.5f;
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
	texture = "projectile";
	lifetime = 2.0f;
	scale = 0.1f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.8f;

	damage = 0.6f;

	deathColor = glm::vec3(2.0f, 1.0f, 0.0f);
	deathtime = 2.0f;
	Projectile::init();
}