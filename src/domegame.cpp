#include "./domegame.hpp"

DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};

float a = 0.0f;
void DomeGame::render() const{
	glUniform1i(TEX_loc, 0);

	sgct::ShaderManager::instance()->bindShaderProgram("test");

	//bind sceneshader here tho pliz
	glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);
	myScene->render();
	
	DomeDrawable::bindSprite();


	for (int i = 0; i < players.size(); i++) {
		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_RADIUS));
		glm::mat4 rot = players[i]->getRotationMatrix();
		glm::mat4 scale = glm::scale(glm::mat4(), players[i]->getScale()*glm::vec3(1, 1, 1));

		glm::mat4 playerMat = MVP * rot * trans * scale;
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &playerMat[0][0]);
		players[i]->render();

		Weapon* wp = players[i]->getWeapon();
		trans = glm::translate(trans, glm::vec3(0, 0, 0.1f));
		scale = glm::scale(scale, wp->getScale() * glm::vec3(1, 1, 1));
		glm::mat4 weaponMat = MVP * rot *  wp->getRotationMatrix() * trans * scale;
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &weaponMat[0][0]);
		wp->render();
	}

	for (int i = 0; i < projectiles.size(); i++) {
		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_RADIUS));
		glm::mat4 rot = projectiles[i].getRotationMatrix();
		glm::mat4 scale = glm::scale(glm::mat4(), projectiles[i].getScale()*glm::vec3(1, 1, 1));

		glm::mat4 playerMat = MVP * rot * trans * scale;
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &playerMat[0][0]);
		projectiles[i].render();
	}

	glBindVertexArray(0);
	sgct::ShaderManager::instance()->unBindShaderProgram();

}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("background", "../Images/background.png", true);
	sgct::TextureManager::instance()->loadTexture("player", "../Images/avatar3.png", true);
	sgct::TextureManager::instance()->loadTexture("bullet", "../Images/bullet.jpg", true);
	sgct::TextureManager::instance()->loadTexture("venus", "../Images/venus.jpg", true);

	sgct::ShaderManager::instance()->addShaderProgram(
		"test", "../test.vert", "../test.frag");

	sgct::ShaderManager::instance()->bindShaderProgram("test");
	MVP_loc = sgct::ShaderManager::instance()->getShaderProgram("test").getUniformLocation("MVP");
	TEX_loc = sgct::ShaderManager::instance()->getShaderProgram("test").getUniformLocation("TEX");
	sgct::ShaderManager::instance()->unBindShaderProgram();

	// Enable some openGL stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable( GL_LIGHTING );
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Set up backface culling
	glCullFace(GL_BACK);
	// The polygon winding is counter clockwise (CCW)
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	std::cout << "johan was here xD" << std::endl;

	DomeDrawable::initSprite();

	Weapon::init(&projectiles, &added_projectiles);
    
}

void DomeGame::addPlayer(std::string &name, std::string weaponType, glm::quat pos) {
    Player * newPlayer = new Player(name,std::string("player"),pos);
	if(weaponType == "shotgun")
		newPlayer->setWeapon(new Shotgun(newPlayer), "shotgun");
	if (weaponType == "smg")
		newPlayer->setWeapon(new SMG(newPlayer), "smg");
	players.push_back(newPlayer);
	added_players.addVal(*newPlayer);
    std::cout << "Player created" << std::endl;
}

void DomeGame::update(float dt) {
	for (int i = 0; i < players.size(); i++) {
		//players[i]->update(dt * (1.0f - i*0.8f)); to make the second player much slower for testing
		players[i]->update(dt);
		players[i]->getWeapon()->update(dt,players[i]->c_shoot);
	}
	for (int i = 0; i < projectiles.size(); i++) {
		if (!projectiles[i].update(dt))
		{
			projectiles.erase(projectiles.begin() + i);
			removed_projectiles.addVal(i);
			i--;
		}
	}

	for (int i = 0; i < players.size(); i++) {
		for (int k = i + 1; k < players.size(); k++)
		{
			glm::vec3 p1_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 p2_pos = players[k]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 diff = p2_pos - p1_pos;

			//If the players collide: apply knockbacks and place them outside eachothers radius
			if (glm::length(diff) < (players[i]->getScale() + players[k]->getScale())/2) // radius = scale/2
			{
				glm::vec3 p1_w_vel = players[i]->getWorldVelocity();
				glm::vec3 p2_w_vel = players[k]->getWorldVelocity();

				glm::vec3 n_diff = glm::normalize(diff);

				// velocity vector towards eachother
				glm::vec3 p1_w_rel_vel = n_diff * glm::dot(p1_w_vel, n_diff);
				glm::vec3 p2_w_rel_vel = n_diff * glm::dot(p2_w_vel, n_diff);

				// cancel out velocity towards the other
				players[i]->addWorldVelocity(-p1_w_rel_vel);
				players[k]->addWorldVelocity(-p2_w_rel_vel);

				// switch relative velocities
				players[i]->addWorldVelocity(p2_w_rel_vel);
				players[k]->addWorldVelocity(p1_w_rel_vel);

				// move players away from each others radius
				glm::vec3 trans = n_diff *((players[i]->getScale() + players[k]->getScale())/2 - glm::length(diff)) / DOME_RADIUS * 0.50f ; // 0.55f for extra margin
				players[i]->addWorldTranslation(-trans);
				players[k]->addWorldTranslation(trans);
			}
		}
	}
	for (int i = 0; i < players.size(); i++) {
        for(int k = 0; k < projectiles.size(); k++)
        {
			glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 b_pos = projectiles[k].getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 diff = p_pos - b_pos;
            //If the bullet hits: decrease & increase score
            if(projectiles[k].getOwner() != players[i] && glm::length(diff) < (projectiles[k].getScale() + players[i]->getScale())/2)
            {
				players[i]->addWorldVelocity(projectiles[k].getWorldVelocity() * 0.05f);
                players[i]->decreaseScore();
                projectiles[k].getOwner()->increaseScore();
                projectiles.erase(projectiles.begin() + k);
				removed_projectiles.addVal(k);

            }
        }
	}
}
