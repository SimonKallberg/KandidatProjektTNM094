#include "DomeDrawable.hpp"
//Initializing static variable
Quad* DomeDrawable::sprite = nullptr;

void DomeDrawable::addWorldTranslation(glm::vec3 w_trans) {
	glm::vec3 local_trans = glm::inverse(position) * w_trans;
	position *= glm::quat(glm::vec3(local_trans.y, 0.0f, 0.0f));
	position *= glm::quat(glm::vec3(0.0f, -local_trans.x, 0.0f));
	position = glm::normalize(position);
}

void DomeDrawable::setPosition(glm::quat pos) {
	position = pos;
}

void DomeDrawable::setTexture(std::string tex) {
    texture = tex;
}

void DomeDrawable::render() const {
    sprite->bindVAO();
    sprite->setTexture(texture, bumpTexture);
    sprite->render();
}

float DomeDrawable::getScale() const {
    return scale;
}

float DomeDrawable::getDirection() const {
    return direction;
}

glm::quat DomeDrawable::getQuat() const {
    return position * glm::quat(glm::vec3(0, 0, -direction));
}

//Static functions
void DomeDrawable::initSprite() {
    sprite = new Quad("venus", 1.0f);
}

void DomeDrawable::bindSprite() {
    sprite->bindVAO();
}
//Virtual functions

void DomeDrawable::writeData() {
    
    
    sgct::SharedObject<glm::quat> s_pos;
    s_pos.setVal(position);
	sgct::SharedData::instance()->writeObj(&s_pos);

	sgct::SharedFloat s_dir = direction;
	sgct::SharedData::instance()->writeFloat(&s_dir);

	sgct::SharedFloat s_scale = scale;
	sgct::SharedData::instance()->writeFloat(&s_scale);

	sgct::SharedString s_tex = texture;
	sgct::SharedData::instance()->writeString(&s_tex);
}

void DomeDrawable::readData() {
	sgct::SharedObject<glm::quat> s_pos;
	sgct::SharedData::instance()->readObj(&s_pos);
	position = s_pos.getVal();

	sgct::SharedFloat s_dir;
	sgct::SharedData::instance()->readFloat(&s_dir);
	direction = s_dir.getVal();

	sgct::SharedFloat s_scale;
	sgct::SharedData::instance()->readFloat(&s_scale);
	scale = s_scale.getVal();

	sgct::SharedString s_tex;
	sgct::SharedData::instance()->readString(&s_tex);
	texture = s_tex.getVal();

}

glm::mat4 DomeDrawable::getRotationMatrix() const {
    return glm::toMat4(getQuat());
}

