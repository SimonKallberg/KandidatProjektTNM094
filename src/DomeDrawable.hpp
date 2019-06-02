#ifndef DomeDrawable_hpp
#define DomeDrawable_hpp

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include "./sgct.h"
#include "./Quad.hpp"
#include <algorithm>

class DomeDrawable {
public:

    //Constructors
	DomeDrawable() = delete;
	DomeDrawable(std::string &tex, glm::quat pos = glm::quat()) : texture(tex), position(pos) {};

    //Public member functions
    //Apply a translation from the xyz world to the rotational position. Orthogonal part to dome(on current pos) is ignored. The parallel part is in rotation units(rads).
	void addWorldTranslation(glm::vec3 w_trans);
    void setPosition(glm::quat pos);
    void setTexture(std::string tex);
    void render() const;
	float getScale() const;
	float getDirection() const;
    glm::quat getQuat() const;
    
    //Static functions
	static void initSprite();
	static void bindSprite();
    
    //Virtual functions
	virtual void writeData();
	virtual void readData();
    virtual glm::mat4 getRotationMatrix() const;

protected:
    //The quad that renders on the screen
	static Quad * sprite;
    
    //Texture & bumptexture
	std::string texture;
	std::string bumpTexture = "NOBUMP";
    
    //Position, direction and scale
	glm::quat position;
	float direction = 0; //angle(radians)  0 = straight up, 90 = right
	float scale = 1.0f;

};





#endif
