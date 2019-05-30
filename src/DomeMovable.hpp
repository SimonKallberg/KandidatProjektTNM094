#ifndef DomeMovable_hpp
#define DomeMovable_hpp

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include "./Quad.hpp"
#include "./DomeDrawable.hpp"

class DomeMovable : public DomeDrawable {
public:

    //Constructors
	DomeMovable() = delete;
	DomeMovable(std::string &tex, glm::quat pos = glm::quat()) : DomeDrawable(tex, pos) {};
    
    //Velocities
    // Apply a velocity from the xyz world to the local velocity. Orthogonal part to dome is ignored
	void addWorldVelocity(glm::vec3 w_vel);
    // In rotational units, but aligned with the XYZ world
    glm::vec3 getWorldVelocity();

	//Update function. Moves the Movable by it's current velocities and slowdown
	void update(float dt);

protected:

	// Velocities
	float up_vel = 0.0f;
	float right_vel = 0.0f;
    // Speed multiplier per second, if no acceleration then after one second velocity = velocity * slowdown;
	float slowdown = 1.0f;
};

#endif
