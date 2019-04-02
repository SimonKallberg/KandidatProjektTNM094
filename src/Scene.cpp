#include "./Scene.hpp"
#include <iostream>

// Constructor
Scene::Scene(){
	std::cout << "scene gjord" << std::endl;
    background = new Quad("background", 2.5f, 1.5f);
}


// Draws the scene in dome or normal desktop mode
void Scene::draw() const {
	//background->display();
	//std::cout << "drawing scene..." << std::endl;
    background->draw(0.0f, 0.0f, -0.4f);
}
