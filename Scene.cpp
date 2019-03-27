#include "./Scene.hpp"
#include <iostream>

// Constructor
Scene::Scene(){
	std::cout << "scene gjord" << std::endl;
    background = new Quad("avatar3.png", 1.0f, 1.0f);
}


// Draws the scene in dome or normal desktop mode
void Scene::draw() const {
	//background->display();
	std::cout << "drawing scene..." << std::endl;
    background->draw(0.0f, 0.0f, -0.01f);
}
