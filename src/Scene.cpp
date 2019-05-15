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
	sceneshader.ambient = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("ambient");

	for (int i = 0; i < N_LIGHTS; i++) {
		sceneshader.light_pos_loc[i] = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("pointLights[" + std::to_string(i) + "].position");
		sceneshader.light_color_loc[i] = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("pointLights[" + std::to_string(i) + "].color");
	}


	sceneshader.s_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("scene").getUniformLocation("s_tex");

	ModelLoader * earth = new ModelLoader("../Objects/lowpoly_earth", "earth");
	ModelLoader * venus = new ModelLoader("../Objects/sharednormalsphere", "venus");
	venus->bumpTextureName = "spherebump";

	Body * temp;

	systems.push_back(Body(venus));
	temp = &systems[0];
	temp->ambient = glm::vec3(1, 1, 1);
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(20.0f, 0.0f, -60.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	temp->subBodies.push_back(Body(earth, temp));
	
	temp = &temp->subBodies[0];
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(50.0f, 0.0f, 0.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	

	systems.push_back(Body(nullptr));
	temp = &systems[1];

	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(10.0f, 20.0f, -10.0f));

	temp->subBodies.push_back(Body(venus, temp));
	temp->subBodies[0].localTransformation = glm::translate(glm::mat4(), glm::vec3(10.0f, 00.0f, 0.0f));
	temp->subBodies[0].selfTransformation = glm::scale(glm::mat4(), 0.1f * glm::vec3(1.0f, 1.0f, 1.0f));
	
	temp->subBodies.push_back(Body(venus, temp));
	temp->subBodies[1].localTransformation = glm::translate(glm::mat4(), glm::vec3(-20.0f, 0.0f, -10.0f));
	temp->subBodies[1].selfTransformation = glm::scale(glm::mat4(), 0.04f * glm::vec3(1.0f, 1.0f, 1.0f));

	temp->subBodies.push_back(Body(venus, temp));
	temp->subBodies[2].localTransformation = glm::translate(glm::mat4(), glm::vec3(20.0f, 0.0f, -10.0f));
	temp->subBodies[2].selfTransformation = glm::scale(glm::mat4(), 0.2f * glm::vec3(1.0f, 1.0f, 1.0f));


}

void Scene::update(float dt) {
	Body * temp = &systems[0].subBodies[0];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 10.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 10.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[1].subBodies[0];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 50.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 10.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[1].subBodies[1];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 1.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[1].subBodies[2];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 30.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 2.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

}


// Draws the scene in dome or normal desktop mode
void Scene::render() const {
	sgct::ShaderManager::instance()->bindShaderProgram("scene");
	glUniform1i(sceneshader.d_tex_loc, 0);
	glUniform1i(sceneshader.b_tex_loc, 1);
	glUniform1i(sceneshader.s_tex_loc, 2);
	glUniformMatrix4fv(sceneshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(sceneshader.light_pos_loc[i], 1, &lightpos[0]);
		glUniform3fv(sceneshader.light_color_loc[i], 1, &lightpos[0]);
	}
	
	glm::vec3 lightpos = systems[0].getCentre();
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(sceneshader.light_pos_loc[0], 1, &lightpos[0]);
	glUniform3fv(sceneshader.light_color_loc[0], 1, &color[0]);

	glm::mat4 origin;
	for each  (Body b in systems)
	{
		b.render(origin);
	}
	sgct::ShaderManager::instance()->unBindShaderProgram();

}
