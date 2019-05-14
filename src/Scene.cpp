#include "./Scene.hpp"


Scene::SceneShader Scene::sceneshader = Scene::SceneShader();

// Constructor
Scene::Scene(){
	std::cout << "scene gjord" << std::endl;
    //background = new Quad("background", 2.5f, 1.5f);

}

void Scene::initScene() {
	sgct::ShaderManager::instance()->addShaderProgram(
		"scene", "../scene.vert", "../scene.frag");

	sgct::ShaderManager::instance()->bindShaderProgram("scene");
	sceneshader.MVP_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("MVP");
	sceneshader.model_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("model");
	sceneshader.d_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("d_tex");
	sceneshader.b_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("b_tex");
	sceneshader.s_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("s_tex");

	std::cout << sceneshader.MVP_loc << "\n";
	std::cout << sceneshader.d_tex_loc << "\n";
	std::cout << sceneshader.b_tex_loc << "\n";
	std::cout << sceneshader.s_tex_loc << "\n";
	
	systems.push_back(Body("../Objects/box", "venus"));
	Body * temp = &systems[0];
	temp->localTransformation = glm::scale(glm::translate(glm::mat4(), glm::vec3(20.0f, 0.0f, -50.0f)), glm::vec3(20,20,20));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));
	temp->subBodies.push_back(Body("../Objects/box", "bullet"));
	
	temp = &temp->subBodies[0];
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, 0.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f));
}

void Scene::update(float dt) {
	Body * temp = &systems[0];


}



// Draws the scene in dome or normal desktop mode
void Scene::render() const {
	sgct::ShaderManager::instance()->bindShaderProgram("scene");
	glUniform1i(sceneshader.d_tex_loc, 0);
	glUniform1i(sceneshader.b_tex_loc, 1);
	glUniform1i(sceneshader.s_tex_loc, 2);
	glUniformMatrix4fv(sceneshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 origin;
	for each  (Body b in systems)
	{
		b.render(origin);
	}
	sgct::ShaderManager::instance()->unBindShaderProgram();

}
