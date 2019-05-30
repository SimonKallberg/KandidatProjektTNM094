#include "./domegame.hpp"


DomeGame::DomeGame(sgct::Engine * gEngine, std::string rootDir_in) {		//Constructor
    rootDir = rootDir_in;
    myScene = new Scene(rootDir);
	std::cout << "DomeGame gjord" << std::endl;
};

float a = 0.0f;
void DomeGame::render() const{

	myScene->MVP = MVP;
	myScene->render();

	sgct::ShaderManager::instance()->bindShaderProgram("player");
	glUniform1i(playershader.d_tex_loc, 0);
	glUniform1i(playershader.b_tex_loc, 1);
	glUniformMatrix4fv(playershader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);
	
	DomeDrawable::bindSprite();

	//std::cout << "x: " << a.x << "  y: " << a.y << "  z: " << a.z << std::endl;
	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(playershader.light_pos_loc[i], 1, &lightpos[0]);
		glm::vec3 color(0, 0, 0);
		glUniform3fv(playershader.light_color_loc[i], 1, &color[0]);
	}
		

	for (int i = 0; i < players.size(); i++) {
		glm::vec3 i_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);

		//glm::vec3 lightpos = players[i]->getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.2f));
		//glUniform3fv(light_pos_loc[0], 1, &lightpos[0]);

		int lightindex = 0;
		for (int j = 0; lightindex < N_LIGHTS && j < projectiles.size(); j++) {
			glm::vec3 j_pos = projectiles[j].getQuat() * glm::vec3(0, 0, -DOME_RADIUS);

			if (glm::length(i_pos - j_pos) < 3.0f) { // render if light is close enough
				glm::vec3 lightpos = projectiles[j].getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.1f));
				glUniform3fv(playershader.light_pos_loc[lightindex], 1, &lightpos[0]);

				glm::vec3 color = projectiles[j].getLightColor();
				glUniform3fv(playershader.light_color_loc[lightindex], 1, &color[0]);

				lightindex++;
			}
		}
		
		renderPlayer(players[i]);
	}

	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(playershader.light_pos_loc[i], 1, &lightpos[0]);
		glm::vec3 color(0, 0, 0);
		glUniform3fv(playershader.light_color_loc[i], 1, &color[0]);
	}

	for (int i = 0; i < players.size(); i++) {
		glm::vec3 i_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);

		//glm::vec3 lightpos = players[i]->getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.2f));
		//glUniform3fv(light_pos_loc[0], 1, &lightpos[0]);

		int lightindex = 0;
		for (int j = 0; lightindex < N_LIGHTS && j < projectiles.size(); j++) {
			glm::vec3 j_pos = projectiles[j].getQuat() * glm::vec3(0, 0, -DOME_RADIUS);

			if (glm::length(i_pos - j_pos) < 3.0f) { // render if light is close enough
				glm::vec3 lightpos = projectiles[j].getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.2f));
				glUniform3fv(playershader.light_pos_loc[lightindex], 1, &lightpos[0]);

				glm::vec3 color = projectiles[j].getLightColor();
				glUniform3fv(playershader.light_color_loc[lightindex], 1, &color[0]);

				lightindex++;
			}
		}


		renderWeapon(players[i]);
	}


	sgct::ShaderManager::instance()->bindShaderProgram("projectile");
	glUniform1i(projectileshader.d_tex_loc, 0);
	glUniformMatrix4fv(projectileshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

	for (int i = 0; i < projectiles.size(); i++) {
		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -(DOME_RADIUS - 0.2f)));
		glm::mat4 rot = projectiles[i].getRotationMatrix();
		glm::mat4 scale = glm::scale(glm::mat4(), projectiles[i].getScale()*glm::vec3(1, 1, 1));

		glm::mat4 playerMat = rot * trans * scale;
		glUniformMatrix4fv(projectileshader.model_loc, 1, GL_FALSE, &playerMat[0][0]);
		glm::vec4 color = projectiles[i].getColor();
		glUniform4fv(projectileshader.color, 1, &color[0]);
		projectiles[i].render();
	}
    
	glBindVertexArray(0);
    
    //Print highscore
	sgct::ShaderManager::instance()->unBindShaderProgram();
    glm::mat4 scoreMat = MVP * glm::rotate(glm::mat4(), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::translate(glm::mat4(), glm::vec3(0.0f, 13.0f, -20.0f))
    * glm::scale(glm::mat4(), glm::vec3(1.0f));
    sgct_text::print3d(sgct_text::FontManager::instance()->getFont("Verdana", 30), sgct_text::TOP_LEFT, scoreMat,  "HIGHSCORE");
    sgct_text::print3d(sgct_text::FontManager::instance()->getFont("Verdana", 14), sgct_text::TOP_LEFT, scoreMat, scoreboard.c_str());

}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("background", rootDir + "/Images/background.png", true);
	sgct::TextureManager::instance()->loadTexture("bullet", rootDir + "/Images/WEAPON.png", true);
	sgct::TextureManager::instance()->loadTexture("venus", rootDir + "/Images/venus.jpg", true);
	sgct::TextureManager::instance()->loadTexture("earth", rootDir + "/Images/earth_texture.png", true);
	sgct::TextureManager::instance()->loadTexture("testbump", rootDir + "/Images/testbump.png", true);
	sgct::TextureManager::instance()->loadTexture("spherebump", rootDir + "/Images/spherebump.png", true);
	sgct::TextureManager::instance()->loadTexture("NOBUMP", rootDir + "/Images/nobump.png", true);
	sgct::TextureManager::instance()->loadTexture("projectile", rootDir + "/Images/projectile.png", true);
	
	for (int i = 0; i <= 9; i++)
		sgct::TextureManager::instance()->loadTexture("player" + std::to_string(i), rootDir + "/Images/avatar" + std::to_string(i) + ".png", true);
	sgct::TextureManager::instance()->loadTexture("playerbump", rootDir + "/Images/avatarbump2.png", true);

	sgct::TextureManager::instance()->loadTexture("weapon1", rootDir + "/Images/Weapon_1.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon1normal", rootDir + "/Images/Weapon1_normalmap.png", true);

	// PlayerShader
	sgct::ShaderManager::instance()->addShaderProgram(
		"player", rootDir + "/player.vert", rootDir + "/player.frag");

	sgct::ShaderManager::instance()->bindShaderProgram("player");
	playershader.MVP_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("MVP");
	playershader.model_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("model");
	playershader.d_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("d_tex");
	playershader.b_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("b_tex");

	for (int i = 0; i < N_LIGHTS; i++) {
		playershader.light_pos_loc[i] = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("pointLights[" + std::to_string(i) + "].position");
		playershader.light_color_loc[i] = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("pointLights[" + std::to_string(i) + "].color");
	}


	// ProjectileShader
	sgct::ShaderManager::instance()->addShaderProgram(
		"projectile", rootDir + "/projectile.vert", rootDir + "/projectile.frag");

	sgct::ShaderManager::instance()->bindShaderProgram("projectile");
	projectileshader.MVP_loc = sgct::ShaderManager::instance()->getShaderProgram("projectile").getUniformLocation("MVP");
	projectileshader.model_loc = sgct::ShaderManager::instance()->getShaderProgram("projectile").getUniformLocation("model");
	projectileshader.d_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("projectile").getUniformLocation("d_tex");
	projectileshader.color = sgct::ShaderManager::instance()->getShaderProgram("projectile").getUniformLocation("color");


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


	myScene->initScene();

	DomeDrawable::initSprite();

	Weapon::init(&projectiles, &added_projectiles);
    
}

void DomeGame::addPlayer(std::string &name, std::string weaponType, glm::quat pos) {
    std::string texName = "player" + std::to_string(players.size()+1);
    Player * newPlayer = new Player(name, texName,pos);
	if(weaponType == "shotgun")
		newPlayer->setWeapon(new Shotgun(newPlayer), "shotgun");
	if (weaponType == "smg")
		newPlayer->setWeapon(new SMG(newPlayer), "smg");
	if (weaponType == "light")
		newPlayer->setWeapon(new LightBallLauncher(newPlayer), "light");
	players.push_back(newPlayer);
	added_players.addVal(*newPlayer);
    std::cout << "Player" << name << " created" << std::endl;
}

void DomeGame::renderPlayer(Player *p) const {
	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_RADIUS));
	glm::mat4 rot = p->getRotationMatrix();
	glm::mat4 scale = glm::scale(glm::mat4(), p->getScale()*glm::vec3(1, 1, 1));

	glm::mat4 playerMat = rot * trans * scale;
	glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &playerMat[0][0]);
	p->render();

}

void DomeGame::renderWeapon(Player *p) const {
	Weapon* wp = p->getWeapon();
	float pScale = p->getScale();

	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -(DOME_RADIUS - 0.1f)));
	glm::mat4 rot = p->getRotationMatrix() * wp->getRotationMatrix();
	glm::mat4 scale = glm::scale(glm::mat4(), pScale * wp->getScale()*glm::vec3(1, 1, 1));

	glm::mat4 weaponMat = rot * trans * scale;
	glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &weaponMat[0][0]);
	wp->render();

}

void DomeGame::update(float dt) {

	myScene->update(dt);

	for (int i = 0; i < players.size(); i++) {
		//players[i]->update(dt * (1.0f - i*0.8f)); to make the second player much slower for testing
		players[i]->update(dt);
		players[i]->getWeapon()->update(dt,players[i]->c_shoot);
		glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
		if (p_pos.z > 2.0f) {
			players[i]->addWorldVelocity(0.01f * glm::vec3(0, 0, -1.0f));
		}
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
			if (glm::length(diff) < (players[i]->getScale() + players[k]->getScale())/2.4f) // radius = scale/2
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
				glm::vec3 trans = n_diff *((players[i]->getScale() + players[k]->getScale())/2.4f - glm::length(diff)) / DOME_RADIUS * 0.50f ; // 0.55f for extra margin
				players[i]->addWorldTranslation(-trans);
				players[k]->addWorldTranslation(trans);
			}
		}
	}
    
	for (int i = 0; i < players.size(); i++) {
        for(int k = 0; k < projectiles.size(); k++)
        {
			if (projectiles[k].alive()) {
				glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
				glm::vec3 b_pos = projectiles[k].getQuat() * glm::vec3(0, 0, -DOME_RADIUS);
				glm::vec3 diff = p_pos - b_pos;
				//If the bullet hits: decrease & increase score
				if (projectiles[k].getOwner() != players[i] && glm::length(diff) < (projectiles[k].getScale() + players[i]->getScale()) / 2.2f)
				{
					glm::vec3 n_diff = glm::normalize(diff);
					glm::vec3 knockback = n_diff * glm::dot(projectiles[k].getWorldVelocity(), n_diff);

					if (players[i]->isAlive()) {
						players[i]->addWorldVelocity(projectiles[k].getWorldVelocity() * projectiles[k].damage);
						projectiles[k].getOwner()->increaseScore(projectiles[k].damage);
					}
					
					projectiles[k].collide();
				}
			}
        }
	}
    
    scoreboard = getScoreboard();
}

std::string DomeGame::getScoreboard() {
    
    std::vector<playerScore> playerScoreList;
    std::string totalScore = "";
    
    for(int i = 0; i < players.size(); i++)
    {
        playerScoreList.push_back(playerScore());
        playerScoreList[i].playerName = players[i]->playerName;
        playerScoreList[i].score = players[i]->score;
    }
	sort(playerScoreList.begin(), playerScoreList.end()); 
    
    int playerSize = playerScoreList.size();
    int printAmount = std::fmin(playerSize, 10);
    
    std::stringstream scoreStream;
    
    scoreStream << std::endl;
    
    
    for(int i = 0; i < printAmount ; i++)
    {
        
        scoreStream << std::left << std::setfill(' ') << std::setw(10) << playerScoreList[i].score;
        scoreStream << std::left << std::setfill(' ') << std::setw(30) << playerScoreList[i].playerName << std::endl;
        
    }
    scoreStream << std::endl;
    
    totalScore = scoreStream.str();
    
    return totalScore;
}

bool operator<(const DomeGame::playerScore &a, const DomeGame::playerScore &b)
{
    return a.score > b.score;
}
