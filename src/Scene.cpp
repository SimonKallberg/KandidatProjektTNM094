#include "./Scene.hpp"

// Constructor
Scene::Scene(){
	std::cout << "scene gjord" << std::endl;
    //background = new Quad("background", 2.5f, 1.5f);

	test_bg = new ModelLoader("../Objects/sphere", "background");
	test_star = new ModelLoader("../Objects/lowpoly", "venus");
}

float BG_radius = 20.0f;

// Draws the scene in dome or normal desktop mode
void Scene::render() const {

	test_bg->draw(BG_radius);
	test_star->draw(0.2f,glm::vec3(2,0,-4));
	glPopMatrix();

}
