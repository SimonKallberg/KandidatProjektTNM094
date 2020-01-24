#include "./Scene.hpp"

Scene::SceneShader Scene::sceneshader = Scene::SceneShader();

// Constructor
Scene::Scene(std::string rootDir_in)
{
    rootDir = rootDir_in;
	std::cout << "scene gjord" << std::endl;
    //background = new Quad("background", 2.5f, 1.5f);

}

Scene::~Scene() {
	delete models.background;
	delete models.dangerzone;
	delete models.dark;
	delete models.earth;
	delete models.lava;
	delete models.moon;
	delete models.pink;
	delete models.venus;
}

void Scene::initScene() {
	sgct::ShaderManager::instance()->addShaderProgram(
		"scene", rootDir + "/scene.vert", rootDir + "/scene.frag");

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

	models.earth = new ModelLoader( rootDir + "/Objects/lowpoly_earth", "earth");		
	models.venus = new ModelLoader(rootDir + "/Objects/sharednormalsphere", "venus");
	models.background = new ModelLoader(rootDir + "/Objects/sphere", "venus");
	models.lava = new ModelLoader(rootDir + "/Objects/lowpoly_lavaplanet", "lava");
	models.pink = new ModelLoader(rootDir + "/Objects/lowpoly_pinkplanet", "pink");
	models.dark = new ModelLoader(rootDir + "/Objects/lowpoly_darkplanet", "dark");
	models.moon = new ModelLoader(rootDir + "/Objects/lowpoly_moon", "moon");
	models.pinkmoon = new ModelLoader(rootDir + "/Objects/lowpoly_moon", "pink");

	models.dangerzone = new ModelLoader(rootDir + "/Objects/sphere", "NOBUMP");

	models.earth->specularTextureName = "earthlava_spec";
	models.lava->specularTextureName = "earthlava_spec";
	models.pink->specularTextureName = "darkpink_spec";
	models.dark->specularTextureName = "darkpink_spec";

	//lava->bumpTextureName = "spherebump";

	Body * temp;

	// place background first for the render loop not to apply light from sources
	systems.push_back(Body(models.background));
	//systems[0].localTransformation = glm::translate(glm::mat4(), glm::vec3(30.0f, 20.0f, -50.0f));
	systems[0].selfTransformation = glm::scale(glm::mat4(), 100.0f * glm::vec3(1.0f, 1.0f, 1.0f));
	systems[0].ambient = glm::vec3(0.1f, 0.1f, 0.4f);

	// and dangerzone
	systems.push_back(Body(models.dangerzone));
	systems[1].selfTransformation = glm::scale(glm::mat4(), 6.0f * glm::vec3(1.0f, 1.0f, 1.0f))
								  * glm::rotate(glm::mat4(), 0.8f, glm::vec3(-1.0f, 0.0f, 0.0f));
	systems[1].ambient = glm::vec3(1.0f);

	systems.push_back(Body(models.pink));
	temp = &systems[2];
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(30.0f, 20.0f, -50.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(1.9f));

	temp->subBodies.push_back(Body(models.pinkmoon));
	temp = &temp->subBodies[0];
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(-18.0f, 0.0f, 0.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(3.5f));

	systems.push_back(Body(models.earth));
	temp = &systems[3];
	temp->localTransformation = glm::translate(glm::mat4(), glm::vec3(-5.0f, 5.0f, -9.0f));
	temp->selfTransformation = glm::scale(glm::mat4(), glm::vec3(0.3f));

	temp->subBodies.push_back(Body(models.moon));
	temp->subBodies[0].localTransformation = glm::translate(glm::mat4(), glm::vec3(-20.0f, 0.0f, -3.0f));
	temp->subBodies[0].selfTransformation = glm::scale(glm::mat4(), glm::vec3(3.5f));

	systems.push_back(Body(models.lava));
	systems[4].localTransformation = glm::translate(glm::mat4(), glm::vec3(0.0f, 15.0f, -5.0f));
	systems[4].selfTransformation = glm::scale(glm::mat4(), glm::vec3(0.6f));

	systems.push_back(Body(models.dark));
	systems[5].localTransformation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -8.0f));
	systems[5].selfTransformation = glm::scale(glm::mat4(), glm::vec3(0.2f));
	
	for (int i = 0; i < systems.size(); i++) {
		systems[i].initParents();
	}

	std::cout << "SCENE INIT\n";
}

void Scene::update(float dt) {
	lighttimer += dt/2.0f;

	Body * temp;

	temp = &systems[2];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 60.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[2].subBodies[0];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[3];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 60.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;


	temp = &systems[3].subBodies[0];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 40.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 20.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	temp = &systems[4];
//	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 70.0f, glm::vec3(0, 1, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 30.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	systems[4].ambient = glm::vec3(0.7f + 0.2f*cos(lighttimer), 0.4f, 0.4f);

	temp = &systems[5];
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 50.0f, glm::vec3(1, 0.3f, 0)) * temp->localTransformation;
	temp->localTransformation = glm::rotate(glm::mat4(), dt * 3.14f / 73.0f, glm::vec3(0, 0.6f, 0)) * temp->localTransformation;
	temp->selfTransformation = glm::rotate(glm::mat4(), -dt * 3.14f / 30.0f, glm::vec3(0, 1, 0)) * temp->selfTransformation;

	//std::cout << glm::length(systems[3].subBodies[0].getCentre()) << std::endl;
}

//shareddata
void Scene::writeData() {
	sgct::SharedFloat s_timer = lighttimer;
	sgct::SharedData::instance()->writeFloat(&s_timer);

    for (int i = 0; i < systems.size(); i++) {
        systems[i].writeData();
    }
}

void Scene::readData() {
	sgct::SharedFloat s_timer;
	sgct::SharedData::instance()->readFloat(&s_timer);
	lighttimer = s_timer.getVal();

    for (int i = 0; i < systems.size(); i++) {
        systems[i].readData();
    }
}

// Draws the scene in dome or normal desktop mode
void Scene::render() const {
	sgct::ShaderManager::instance()->bindShaderProgram("scene");
	glUniform1i(sceneshader.d_tex_loc, 0);
	glUniform1i(sceneshader.b_tex_loc, 1);
	glUniform1i(sceneshader.s_tex_loc, 2);
	glUniformMatrix4fv(sceneshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 origin;


	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(sceneshader.light_pos_loc[i], 1, &lightpos[0]);
		glUniform3fv(sceneshader.light_color_loc[i], 1, &lightpos[0]);
	}
	systems[0].render(origin);
	

	// dome center lighting
	glm::vec3 lightpos = systems[0].getCentre();
	glm::vec3 color = glm::vec3(0.1f);
	glUniform3fv(sceneshader.light_pos_loc[0], 1, &lightpos[0]);
	glUniform3fv(sceneshader.light_color_loc[0], 1, &color[0]);


	// lava planet lighting
	lightpos = systems[3].getCentre();
	color = (0.8f + 0.2f*cos(lighttimer)) * glm::vec3(1.0f, 0.0f, 0.0f) + (0.45f - 0.15f*cos(lighttimer)) * glm::vec3(0.0f, 1.0f, 1.0f);
	glUniform3fv(sceneshader.light_pos_loc[1], 1, &lightpos[0]);
	glUniform3fv(sceneshader.light_color_loc[1], 1, &color[0]);


	for (int i = 2; i < systems.size(); i++) {
		systems[i].render(origin);
	}

}

void Scene::renderDangerzone() const {
		sgct::ShaderManager::instance()->bindShaderProgram("scene");
		glUniform1i(sceneshader.d_tex_loc, 0);
		glUniform1i(sceneshader.b_tex_loc, 1);
		glUniform1i(sceneshader.s_tex_loc, 2);
		glUniformMatrix4fv(sceneshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

		glm::mat4 origin;

		for (int i = 0; i < N_LIGHTS; i++) {
			glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
			glUniform3fv(sceneshader.light_pos_loc[i], 1, &lightpos[0]);
			glUniform3fv(sceneshader.light_color_loc[i], 1, &lightpos[0]);
		}
		systems[1].render(origin);

		sgct::ShaderManager::instance()->unBindShaderProgram();
}

//bodies
void Scene::Body::render(glm::mat4 parentTransformation) const {
	glm::mat4 trans = parentTransformation * localTransformation;
	for (int i = 0; i < subBodies.size(); i++) {
		subBodies[i].render(trans);
	}

	trans *= selfTransformation;
	glUniformMatrix4fv(sceneshader.model_loc, 1, GL_FALSE, &trans[0][0]);
	glUniform3fv(sceneshader.ambient, 1, &ambient[0]);
	if (model)
		model->draw();
}

void Scene::Body::writeData() {
    sgct::SharedObject<glm::mat4> local;
    local.setVal(localTransformation);
    sgct::SharedObject<glm::mat4> self;
    self.setVal(selfTransformation);
	sgct::SharedObject<glm::vec3> s_ambient;
	s_ambient.setVal(ambient);
    
    sgct::SharedData::instance()->writeObj(&local);
    sgct::SharedData::instance()->writeObj(&self);
	sgct::SharedData::instance()->writeObj(&s_ambient);
    
    for (int i = 0; i < subBodies.size(); i++) {
        subBodies[i].writeData();
    }
}

void Scene::Body::readData() {
	sgct::SharedObject<glm::mat4> local;
	sgct::SharedData::instance()->readObj(&local);
	sgct::SharedObject<glm::mat4> self;
	sgct::SharedData::instance()->readObj(&self);

	sgct::SharedObject<glm::vec3> s_ambient;
	sgct::SharedData::instance()->readObj(&s_ambient);
	
	localTransformation = local.getVal();
	selfTransformation = self.getVal();
	ambient = s_ambient.getVal();
	
	for (int i = 0; i < subBodies.size(); i++) {
		subBodies[i].readData();
	}
}

void Scene::Body::initParents(Body* par) {
	parent = par;
	for (int i = 0; i < subBodies.size(); i++) {
		subBodies[i].initParents(this);
	}
}

glm::vec3 Scene::Body::getCentre() const {
    glm::vec4 centre;
    centre = localTransformation * selfTransformation * glm::vec4(0, 0, 0, 1);
    Body * temp = parent;
    while (temp) {
        centre = temp->localTransformation * centre;
        temp = temp->parent;
    }
    return glm::vec3(centre.x, centre.y, centre.z);
}







