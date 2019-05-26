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
	
	const static int N_LIGHTS = 2; // needs to be consistent with sceneshader(f and v)
	static struct SceneShader {
		GLint MVP_loc;
		GLint model_loc;
		GLint d_tex_loc;
		GLint b_tex_loc;
		GLint s_tex_loc;
		GLint light_pos_loc[N_LIGHTS];
		GLint light_color_loc[N_LIGHTS];

		GLint ambient;

	} sceneshader;

	class Body {
	public:
		Body(ModelLoader *ml, Body *p = nullptr) {
			model = ml;
			parent = p;
		}

		void render(glm::mat4 parentTransformation) {
			glm::mat4 trans = parentTransformation * localTransformation;
			for each  (Body b in subBodies)
			{
				b.render(trans);
			}

			trans *= selfTransformation;
			glUniformMatrix4fv(sceneshader.model_loc, 1, GL_FALSE, &trans[0][0]);
			glUniform3fv(sceneshader.ambient, 1, &ambient[0]);
			if(model)
				model->draw();
		}

		glm::vec3 getCentre() const{
			glm::vec4 centre;
			centre = localTransformation * selfTransformation * glm::vec4(0,0,0,1);
			Body * temp = parent;
			while (temp) {
				centre = temp->localTransformation * centre;
				temp = temp->parent;
			}
			return glm::vec3(centre.x, centre.y, centre.z);
		}

		ModelLoader *model;
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::mat4 localTransformation; // affects this body and subbodies
		glm::mat4 selfTransformation; // extra transformation applied for this object after the systemtransformation, does not affect subbodies
		std::vector<Body> subBodies;
		Body *parent; // for light positions backtrack
	};

	std::vector<Body> systems;

};

#endif // SCENE_H
