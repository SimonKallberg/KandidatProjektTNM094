#include "./Player.hpp"


Player::Player(std::vector<Bullet*> * inBulletList)
: bullets(inBulletList)
{
    
Player::Player(std::string & name)
: playerName(name)
{
   
}

Player::Player(float in_theta, float in_phi, float in_dir, std::vector<Bullet*> * inBulletList)
: theta(in_theta), phi(in_phi), direction(in_dir), bullets(inBulletList)
{

}

Player::~Player(){
}

void Player::setPosition(float x, float y){
    theta = y; //Placeholders for changing position
    phi = x;
    direction = x+y;
}

void Player::shoot() {
    Bullet * newBullet = new Bullet(phi, theta, direction);
    bullets->push_back(newBullet);
}

void Player::update() {
	direction += turn_speed * (c_right - c_left);
	v_phi = speed * sin(direction);
	v_theta = speed * cos(direction);
	//theta += ; //Placeholders for changing position
	phi += v_phi;
	theta += v_theta;
}

float Player::getTheta(){
    return theta;
}

float Player::getPhi(){
    return phi;
}


void Player::render() const{
    
    //Converting spherical coordinates to cartesian coordinates
	glPushMatrix();
	glRotatef(theta, 1, 0, 0);
	glPushMatrix();
	glRotatef(phi, 0, -1, 0);
	glPushMatrix();
	glTranslatef(0, 0, -RADIUS);
	glPushMatrix();
	glRotatef(direction*180/3.14, 0, 0, -1);
	sprite->render(0, 0, 0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//std::cout << "PHI:" << phi << ", THETA:" << theta << ", DIR:" << direction << "\n";
    //std::cout << x << y << z << std::endl;
    
    // Draws the scene in dome or normal desktop mode
    
    //std::cout << "drawing player..." << std::endl;

   
    
}


