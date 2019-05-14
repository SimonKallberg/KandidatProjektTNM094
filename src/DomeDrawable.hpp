#ifndef DomeDrawable_hpp
#define DomeDrawable_hpp

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include "./sgct.h"
#include "./Quad.hpp"


class DomeDrawable {
public:

	DomeDrawable() = delete;

	DomeDrawable(std::string &tex, glm::quat pos = glm::quat()) : texture(tex), position(pos) {};

	void setPosition(glm::quat pos);

	// apply a translation from the xyz world to the rotational position. Orthogonal part to dome(on current pos) is ignored. The parallel part is in rotation units(rads)
	void addWorldTranslation(glm::vec3 w_trans);

	glm::quat getQuat() const;
	virtual glm::mat4 getRotationMatrix() const;
	float getScale() const;
	float getDirection() const;

	static void initSprite();
	static void bindSprite();
	void render() const;

	virtual void writeData();
	virtual void readData();

	void setTexture(std::string tex);

protected:
	static Quad * sprite;
	std::string texture;
	std::string bumpTexture = "NOBUMP";
	glm::quat position = glm::quat(1, 0, 0, 0);
	float direction = 0; //angle(radians)  0 = straight up, 90 = right

	float scale = 1.0f;

};





#endif