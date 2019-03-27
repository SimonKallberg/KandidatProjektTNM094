//#include "Scene.hpp"
#include <iostream>
#include "Quad.hpp"

#ifndef SCENE
#define SCENE

class Scene {
public:
	// Constructor
	Scene();

	void draw();
private:
	// Quad *background;
};

#endif // SCENE_H

// Constructor
Scene::Scene(){
	std::cout << "scene gjord" << std::endl;
   // background = new Quad("test", 4.0f, 3.0f);
}


// Draws the scene in dome or normal desktop mode
void Scene::draw() {
	std::cout << "drawing scene..." << std::endl;
   //     background->draw(0.0f, 0.0f, -0.01f);
}
