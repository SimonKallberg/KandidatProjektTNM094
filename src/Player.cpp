#include "./Player.hpp"

Player::Player(){
    
}

Player::Player(int in_theta, int in_phi, int in_dir)
: theta(in_theta), phi(in_phi), direction(in_dir)
{

}

Player::~Player(){
}

void Player::setPosition(float x, float y){
    theta += y; //Placeholders for changing position
    phi += x;
    direction += x+y;
}

float Player::getTheta(){
    return theta;
}

float Player::getPhi(){
    return phi;
}

void Player::render() const{
    
    //Converting spherical coordinates to cartesian coordinates
    float x = RADIUS*sin(theta)*cos(phi);
    float y = RADIUS*sin(theta)*sin(phi);
    float z = RADIUS*cos(theta);
    
    std::cout << x << y << z << std::endl;
    
    // Draws the scene in dome or normal desktop mode
    
    std::cout << "drawing player..." << std::endl;
    texture->draw(x, y, -z);
    
}


