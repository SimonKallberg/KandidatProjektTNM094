#include "./Projectile.hpp"

std::string empty_string = "";

void Projectile::init() {
	lifetimeLeft = lifetime;
	deathtimeLeft = deathtime;

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
	return color * 0.5f *alpha;
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

Player* Projectile::getOwner()
{
    return owner;
}



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

	deathColor = glm::vec3(2.0f, 1.0f, 0.0f);
	deathtime = 2.0f;
	Projectile::init();
}






//Shotgun pellet
ShotgunPellet::ShotgunPellet(glm::quat pos, Player * in_owner, float diradd)
	:  Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 0.4f;
	scale = 0.1f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.00f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;

	direction = diradd;

	damage = 40.0f;
	knockback = 0.4f;

	color = glm::vec3(0.8f, 0.5f, 0.5f);
	deathColor = glm::vec3(0.4f, 0.2f, 0.0f);

	slowdown = 0.2f;
	Projectile::init();
}


//PopBall
PopBall::PopBall(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 1.0f;
	scale = 0.1f;
	propulsionSpeed = 1.0f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.5f;

	damage = 20.0f;
	knockback = 0.2f;

	color = glm::vec3(0.4f, 0.8f, 0.5f);
	deathColor = glm::vec3(0.2f, 0.4f, 0.0f);

	deathtime = 0.1f;
	Projectile::init();
}

// Rifle
RifleBullet::RifleBullet(glm::quat pos, Player * in_owner)
	: Projectile(empty_string, pos, in_owner)
{
	texture = "projectile";
	lifetime = 1.0f;
	scale = 0.2f;
	propulsionSpeed = 1.4f;
	sizeSpread = 0.0f;
	directionSpread = 0.0f;
	propulsionSpread = 0.0f;
	slowdown = 0.8f;

	damage = 40.0f;
	knockback = 0.3f;

	color = glm::vec3(0.5f, 0.5f, 0.8f);
	deathColor = glm::vec3(0.2f, 0.0f, 0.4f);

	deathtime = 0.2f;
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
