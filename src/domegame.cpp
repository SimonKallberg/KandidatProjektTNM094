#include "./domegame.hpp"

DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};

void DomeGame::render() const{
	glUniform1i(TEX_loc, 0);

	
    
	sgct::ShaderManager::instance()->bindShaderProgram("test");

	glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);
	myScene->render();
	
	Player::bindSprite();
	for (int i = 0; i < players.size(); i++){
		glm::mat4 trans = glm::translate(MVP * players[i]->getRotationMatrix(), glm::vec3(0, 0, -DOME_RADIUS));
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &trans[0][0]);
        players[i]->render();

    }

	Bullet::bindSprite();
	for (int i = 0; i < bullets.size(); i++) {
		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_RADIUS));
		glm::mat4 rot = bullets[i]->getRotationMatrix();
		glm::mat4 scale = glm::scale(glm::mat4(), bullets[i]->getSize()*glm::vec3(1, 1, 1));

		glm::mat4 playerMat = MVP * rot * trans * scale;
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &playerMat[0][0]);
		bullets[i]->render();
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	//glDisable( GL_LIGHTING );
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Set up backface culling
	glCullFace(GL_BACK);
	// The polygon winding is counter clockwise (CCW)
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	std::cout << "johan was here xD" << std::endl;

	
	Player::initSprite();
	Bullet::initSprite();
    
    std::string test1 = "Ylva1";
    std::string test2 = "Ylva2";
    
    //addPlayer(test1);
    //addPlayer(test2);
}

void DomeGame::addPlayer(std::string &name) {
    Player * newPlayer = new Player(name, &bullets);
    players.push_back(newPlayer);
    std::cout << "Player created" << std::endl;
}

void DomeGame::update(float dt) {
	for (int i = 0; i < players.size(); i++) {
		//players[i]->update(dt * (1.0f - i*0.8f)); to make the second player much slower for testing
		players[i]->update(dt);
	}
	for (int i = 0; i < bullets.size(); i++) {

		if (!bullets[i]->update(dt))
		{
			bullets.erase(bullets.begin() + i);
		}
	}

	float playerRadius = 0.5f;
	for (int i = 0; i < players.size(); i++) {
		for (int k = i + 1; k < players.size(); k++)
		{
			glm::vec3 p1_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 p2_pos = players[k]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 diff = p2_pos - p1_pos;

			//If the players collide: apply knockbacks and place them outside eachothers radius
			if (glm::length(diff) < 2 * playerRadius) // insert player radius * 2 radius here
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
				glm::vec3 trans = n_diff *(2*playerRadius - glm::length(diff)) / DOME_RADIUS * 0.50f ; // 0.55f for extra margin
				players[i]->addWorldTranslation(-trans);
				players[k]->addWorldTranslation(trans);
			}
		}
	}
	for (int i = 0; i < players.size(); i++) {
        for(int k = 0; k < bullets.size(); k++)
        {
			glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 b_pos = bullets[k]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
			glm::vec3 diff = p_pos - b_pos;
            //If the bullet hits: decrease & increase score
            if(bullets[k]->getOwner() != players[i] && glm::length(diff) < bullets[k]->getSize() + playerRadius) // insert player radius + bullet radius here
            {
				players[i]->addWorldVelocity(bullets[k]->getWorldVelocity() * 0.5f);
                players[i]->decreaseScore();
                bullets[k]->getOwner()->increaseScore();
                bullets.erase(bullets.begin() + k);
            }
        }
	}
}
