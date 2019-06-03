#include "./Projectile.hpp"

std::string empty_string = "";


void Projectile::init() {
	lifetimeLeft = lifetime;
	deathtimeLeft = missDeathTime;

	//proplsion spread
	propulsionSpeed += ((float)rand() / RAND_MAX - 0.5f) * propulsionSpread;
	//direction spread
	direction += ((float)rand() / RAND_MAX - 0.5f) * directionSpread;
	// size spread
	scale += ((float)rand() / RAND_MAX - 0.5f) * sizeSpread;

	float o_scale = owner->getScale();
	scale *= o_scale;
	propulsionSpeed *= owner->getScale();


	//add weapon propulsion
	up_vel += propulsionSpeed * cos(direction);
	right_vel += propulsionSpeed * sin(direction);
}

void Projectile::collide() {
	up_vel = 0;
	right_vel = 0;
	scale *= 1.2f;
	lifetimeLeft = -0.1f;
	deathtimeLeft = hitDeathTime;
	color = hitColor;

	lightColor *= 2;
}

bool Projectile::alive() const { 
    if (lifetimeLeft > 0)
        return true;
    else
        return false;
}

glm::vec4 Projectile::getColor() const {
    return glm::vec4(color, alpha);
}

glm::vec3 Projectile::getLightColor() const {
	return lightColor * alpha;
}

bool Projectile::update(float dt){
	
    if(lifetimeLeft <= 0)
    {
        if(deathtimeLeft <= 0)
			return false;
		
		deathtimeLeft -= dt;

		float smallifier = pow(0.2*missDeathTime, dt);;
		alpha *= smallifier;
		scale *= smallifier;
		color *= smallifier;
		slowdown = 0.1f;
    }

	DomeMovable::update(dt);
	lifetimeLeft -= dt;

    return true;
}

Player* Projectile::getOwner()
{
    return owner;
}


//Shotgun pellet
ShotgunPellet::ShotgunPellet(glm::quat pos, Player * in_owner, float diradd)
	:  Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 0.5f;
	scale = 0.3f;
	propulsionSpeed = 5.0f;
	sizeSpread = 0.005f;
	directionSpread = 0.05f;
	propulsionSpread = 1.0f;
	slowdown = 0.05f;

	direction = diradd;

	damage = 28.0f;
	knockback = 0.14f;

	color = glm::vec3(0.9f, 0.3f, 0.3f);
	hitColor = glm::vec3(1.2f, 1.0f, 0.6f);
	lightColor = glm::vec3(0.6f, 0.2f, 0.1f);

	missDeathTime = 0.4f;
	hitDeathTime = 0.8f;

	Projectile::init();
}


//PopBall
PopBall::PopBall(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 0.4f;
	scale = 0.36f;
	propulsionSpeed = 3.8f;
	sizeSpread = 0.04f;
	directionSpread = 0.16f;
	propulsionSpread = 0.5f;
	slowdown = 0.2f;

	damage = 16.0f;
	knockback = 0.06f;

	color = glm::vec3(0.5f, 0.9f, 0.3f);
	hitColor = glm::vec3(0.6f, 1.2f, 0.2f);
	lightColor = glm::vec3(0.2f, 0.6f, 0.1f);

	missDeathTime = 0.2f;
	hitDeathTime = 0.8f;

	Projectile::init();
}

// Rifle
RifleBullet::RifleBullet(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 0.8f;
	scale = 0.8f;
	propulsionSpeed = 3.0f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.6f;

	damage = 80.0f;
	knockback = 0.5f;

	color = glm::vec3(0.4f, 0.4f, 1.0f);
	hitColor = glm::vec3(0.7f, 0.7f, 1.2f);
	lightColor = glm::vec3(0.4f, 0.4f, 0.9f);

	missDeathTime = 0.3f;
	hitDeathTime = 1.4f;
	Projectile::init();
}





void Projectile::writeData() {
	
	sgct::SharedFloat s_alpha = alpha;
	sgct::SharedData::instance()->writeFloat(&s_alpha);

	sgct::SharedObject<glm::vec3> s_color;
	s_color.setVal(color);
	sgct::SharedData::instance()->writeObj(&s_color);

	DomeDrawable::writeData();
}

void Projectile::readData() {
	sgct::SharedFloat s_alpha;
	sgct::SharedData::instance()->readFloat(&s_alpha);

	sgct::SharedObject<glm::vec3> s_color;
	sgct::SharedData::instance()->readObj(&s_color);
	color = s_color.getVal();

	DomeDrawable::readData();
}



//test projectiles

//SMGRound
SMGRound::SMGRound(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "player2";
	lifetime = 0.5f;
	scale = 0.1f;
	propulsionSpeed = 1.5f;
	sizeSpread = 0.01f;
	directionSpread = 0.05f;
	propulsionSpread = 0.1f;

	damage = 5.0f;
	knockback = 0.02f;

	Projectile::init();
}

//LightBall
LightBall::LightBall(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 2.0f;
	scale = 0.1f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.8f;

	damage = 80.0f;
	knockback = 0.5f;

	hitColor = glm::vec3(2.0f, 1.0f, 0.0f);
	Projectile::init();
}
