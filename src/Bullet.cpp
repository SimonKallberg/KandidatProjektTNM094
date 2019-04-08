#include "./Bullet.hpp"

Bullet::Bullet(float in_phi, float in_theta, float in_direction){
    phi = in_phi;
    theta = in_theta;
    direction = in_direction;
    
}

bool Bullet::update(){
    phi += speed * sin(direction);
    theta += speed * cos(direction);;
    lifeTime -= 0.1f;
    if(lifeTime < 0)
    {
        return false;
    }
    else return true;
}

void Bullet::render(){
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
    
}

float Bullet::getPhi()
{
    return phi;
}
float Bullet::getTheta()
{
    return theta;
}


