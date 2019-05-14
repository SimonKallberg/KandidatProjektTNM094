#include <iostream>
#include "Quad.hpp"
#include "./ModelLoader.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene();

	// move objects around, orbits etc
	void update(float dt);

    void render() const;

	void initScene();

	glm::mat4 MVP;

private:
    Quad *background;
	
	static struct SceneShader {
		GLint MVP_loc;
		GLint model_loc;
		GLint d_tex_loc;
		GLint b_tex_loc;
		GLint s_tex_loc;
	} sceneshader;

	class Body {
	public:
		Body(std::string objpathname, std::string d_tex) 
			: model(objpathname,d_tex)
		{}

		void render(glm::mat4 parentTransformation) {
			glm::mat4 trans = parentTransformation * localTransformation;
			for each  (Body b in subBodies)
			{
				b.render(trans);
			}

			trans *= selfTransformation;
			glUniformMatrix4fv(sceneshader.model_loc, 1, GL_FALSE, &trans[0][0]);
			model.draw();
		}

		ModelLoader model;
		glm::mat4 localTransformation; // affects this body and subbodies
		glm::mat4 selfTransformation; // extra transformation applied for this object after the systemtransformation, does not affect subbodies
		std::vector<Body> subBodies;
	};

	std::vector<Body> systems;

};

#endif // SCENE_H
