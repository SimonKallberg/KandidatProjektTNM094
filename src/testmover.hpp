#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include "./Quad.hpp"

class testmover {

public:

	testmover() {
		pos.x = 0;
		pos.y = 0;
		pos.z = -RADIUS;
		setAngles();
	}
	glm::quat a;
	// moving the player, prob collision testing etc as well
	void update() {
		direction += turn_speed * (c_right - c_left);
		glm::vec3 up(0, cos(phi), sin(phi-3.14/4));
		glm::vec3 right(cos(theta), 0, sin(theta - 3.14 / 4));
		pos += speed*(cos(direction) * up + sin(direction)*right);
		glm::normalize(pos);
		setAngles();
		glm::vec3 EulerAngles(0, 0, 0);
		a = glm::quat(EulerAngles);
		

	}

	void setAngles() {
		phi = atan2(pos.y, pos.z);
		theta = atan2(pos.y, pos.x);
	}

	void render() const
	{
		glm::mat4 m = glm::toMat4(a); glm::to
		glPushMatrix();
		glMultMatrixf();
		glPushMatrix();
		glTranslatef(0, 0, -RADIUS);
		glPushMatrix();
		glRotatef(direction * 180 / 3.14, 0, 0, -1);
		sprite->render(0, 0, 0);
		
		glPopMatrix();
		glPopMatrix();
	}

	float c_left = 0;
	float c_right = 0;


private:
	// player avatar image(sprite is a word for an in game image)
	Quad *sprite = new Quad("player", 0.5f, 0.5f);

	const float RADIUS = 7.4f;

	float speed = 0.2f;
	float turn_speed = 0.05f;

	//Spawns new players at origin
	float theta = 0;
	float phi = 0;

	glm::vec3 pos;

	// velocities
	float v_theta = 0;
	float v_phi = 0;

	float direction = 0; //angle(degrees)  0 = straight up, 90 = right
};