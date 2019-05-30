#include "./DomeMovable.hpp"

void  DomeMovable::addWorldVelocity(glm::vec3 w_vel) {
	glm::vec3 local_vel = glm::inverse(position) * w_vel;
	up_vel += local_vel.y;
	right_vel += local_vel.x;
}

void DomeMovable::update(float dt) {
	up_vel *= pow(slowdown, dt);
	right_vel *= pow(slowdown, dt);

	position *= glm::quat(dt * glm::vec3(up_vel, 0.0f, 0.0f));
	position *= glm::quat(dt * glm::vec3(0.0f, -right_vel, 0.0f));
	position = glm::normalize(position);
}

glm::vec3 DomeMovable::getWorldVelocity() {
    return position * glm::vec3(right_vel, up_vel, 0);
}
