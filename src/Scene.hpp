#include <iostream>
#include "Quad.hpp"
#include "./ModelLoader.hpp"

#ifndef SCENE
#define SCENE

class Scene{
public:
    // Constructor
    Scene(std::string rootDir_in);
	~Scene();
    
    //Public member functions
    void initScene();
	void update(float dt); // move objects around, orbits etc
	void writeData();
    //Shared data for the slaves
	void readData();
	void render() const;
	void renderDangerzone() const;

    //Public member variables
	glm::mat4 MVP;

private:

	struct Models {
		ModelLoader * earth;
		ModelLoader * venus;
		ModelLoader * background;
		ModelLoader * lava;
		ModelLoader * pink;
		ModelLoader * dark;
		ModelLoader * moon;
		ModelLoader * dangerzone;
		ModelLoader * pinkmoon;
	} models;

	float lighttimer = 0;

    //Private class Body
	class Body {
	public:
        
        //Constructor
		Body(ModelLoader *ml) {
			model = ml;
		}
        
        //Public member functions
		void render(glm::mat4 parentTransformation) const;
        void writeData();
        void readData();
        void initParents(Body* par = nullptr);
		glm::vec3 getCentre() const;

        //Public member variables
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::mat4 localTransformation; // affects this body and subbodies
		glm::mat4 selfTransformation; // extra transformation applied for this object after the systemtransformation, does not affect subbodies
		std::vector<Body> subBodies;
        ModelLoader *model;
		Body *parent = nullptr; // for light positions backtrack
	};
    
    //Private member variables
    std::string rootDir = "";
    const static int N_LIGHTS = 2; // needs to be consistent with sceneshader(f and v)
    std::vector<Body> systems;
    
    //Sceneshader struct
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
};

#endif // SCENE_H
