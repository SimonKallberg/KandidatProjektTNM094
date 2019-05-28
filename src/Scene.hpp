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

	void writeData();
	void readData();

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
		Body(ModelLoader *ml) {
			model = ml;
		}

		void render(glm::mat4 parentTransformation) const;

		glm::vec3 getCentre() const;

		void writeData();
		void readData();

		void initParents(Body* par = nullptr);

		ModelLoader *model;
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::mat4 localTransformation; // affects this body and subbodies
		glm::mat4 selfTransformation; // extra transformation applied for this object after the systemtransformation, does not affect subbodies
		std::vector<Body> subBodies;
		Body *parent = nullptr; // for light positions backtrack
	};

	std::vector<Body> systems;

};

#endif // SCENE_H
