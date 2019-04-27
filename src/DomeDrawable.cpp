#include "DomeDrawable.hpp"

void DomeDrawable::addWorldTranslation(glm::vec3 w_trans) {
	glm::vec3 local_trans = glm::inverse(position) * w_trans;
	position *= glm::quat(glm::vec3(local_trans.y, 0.0f, 0.0f));
	position *= glm::quat(glm::vec3(0.0f, -local_trans.x, 0.0f));
	position = glm::normalize(position);
}

void DomeDrawable::setPosition(glm::quat pos) {
	position = pos;
}

glm::quat DomeDrawable::getQuat() const {
	return position * glm::quat(glm::vec3(0, 0, -direction));
}

glm::mat4 DomeDrawable::getRotationMatrix() const {
	return glm::toMat4(getQuat());
}

float DomeDrawable::getScale() const {
	return scale;
}

float DomeDrawable::getDirection() const {
	return direction;
}

void DomeDrawable::render() const {
	sprite->bindVAO();
	sprite->setTexture(texture);
	sprite->render();
};

Quad* DomeDrawable::sprite = nullptr;
void DomeDrawable::initSprite() {
	sprite = new Quad("venus", 1.0f);
}

void DomeDrawable::bindSprite() {
	sprite->bindVAO();
}

void DomeDrawable::setTexture(std::string tex) {
	texture = tex;
}
