#include "./domegame.hpp"


DomeGame::DomeGame(sgct::Engine * gEngine, std::string rootDir_in) {		//Constructor
    rootDir = rootDir_in;
    myScene = new Scene(rootDir);
	std::cout << "DomeGame gjord" << std::endl;
};

DomeGame::~DomeGame() {
	delete myScene;
	for (int i = 0; i < players.size(); i++)
		delete players[i];
	players.clear();

	projectiles.clear();
	added_players.clear();
	added_projectiles.clear();
	changed_weapons.clear();
}

void DomeGame::render() const {

	float projectile_light_reach = 0.8f;

	printScoreboard();

	myScene->MVP = MVP;
	myScene->render();

	//myScene->renderDangerzone();

	for (int i = 0; i < players.size(); i++) {
		renderPlayerName(players[i]);
	}

	sgct::ShaderManager::instance()->bindShaderProgram("player");
	glUniform1i(playershader.d_tex_loc, 0);
	glUniform1i(playershader.b_tex_loc, 1);
	glUniformMatrix4fv(playershader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);
	
	DomeDrawable::bindSprite();

	// reset projectile lighting
	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(playershader.light_pos_loc[i], 1, &lightpos[0]);
		glm::vec3 color(0, 0, 0);
		glUniform3fv(playershader.light_color_loc[i], 1, &color[0]);
	}

	// render the players
	for (int i = 0; i < players.size(); i++) {
		glm::vec3 i_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);

		//glm::vec3 lightpos = players[i]->getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.2f));
		//glUniform3fv(light_pos_loc[0], 1, &lightpos[0]);

		int lightindex = 0;
		for (int j = 0; lightindex < N_LIGHTS && j < projectiles.size(); j++) {
			glm::vec3 j_pos = projectiles[j].getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);

			if (glm::length(i_pos - j_pos) < projectile_light_reach) { // render if light is close enough
				glm::vec3 lightpos = projectiles[j].getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_DRAWABLES_RADIUS - 0.1f));
				glUniform3fv(playershader.light_pos_loc[lightindex], 1, &lightpos[0]);

				glm::vec3 color = projectiles[j].getLightColor();
				glUniform3fv(playershader.light_color_loc[lightindex], 1, &color[0]);

				lightindex++;
			}
		}
		
		glm::vec4 ambient = players[i]->getAmbient();
		glUniform4fv(playershader.ambient, 1, &ambient[0]);

		renderPlayer(players[i]);
	}

	// reset projectile lighting
	for (int i = 0; i < N_LIGHTS; i++) {
		glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(playershader.light_pos_loc[i], 1, &lightpos[0]);
		glm::vec3 color(0, 0, 0);
		glUniform3fv(playershader.light_color_loc[i], 1, &color[0]);
	}

	// render the players weapons (separate loop because transparency draw order) 
	for (int i = 0; i < players.size(); i++) {
		glm::vec3 i_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);

		//glm::vec3 lightpos = players[i]->getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_RADIUS - 0.2f));
		//glUniform3fv(light_pos_loc[0], 1, &lightpos[0]);

		int lightindex = 0;
		for (int j = 0; lightindex < N_LIGHTS && j < projectiles.size(); j++) {
			glm::vec3 j_pos = projectiles[j].getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);

			if (glm::length(i_pos - j_pos) < projectile_light_reach) { // render if light is close enough
				glm::vec3 lightpos = projectiles[j].getQuat() * glm::vec3(0.0f, 0.0f, -(DOME_DRAWABLES_RADIUS - 0.2f));
				glUniform3fv(playershader.light_pos_loc[lightindex], 1, &lightpos[0]);

				glm::vec3 color = projectiles[j].getLightColor();
				glUniform3fv(playershader.light_color_loc[lightindex], 1, &color[0]);

				lightindex++;
			}
		}

		glm::vec4 ambient = players[i]->getAmbient();
		glUniform4fv(playershader.ambient, 1, &ambient[0]);

		renderWeapon(players[i]);
	}


	sgct::ShaderManager::instance()->bindShaderProgram("projectile");
	glUniform1i(projectileshader.d_tex_loc, 0);
	glUniformMatrix4fv(projectileshader.MVP_loc, 1, GL_FALSE, &MVP[0][0]);

	// render the projectiles
	for (int i = 0; i < projectiles.size(); i++) {
		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -(DOME_DRAWABLES_RADIUS - 0.2f)));
		glm::mat4 rot = projectiles[i].getRotationMatrix();
		glm::mat4 scale = glm::scale(glm::mat4(), projectiles[i].getScale()*glm::vec3(1, 1, 1));

		glm::mat4 playerMat = rot * trans * scale;
		glUniformMatrix4fv(projectileshader.model_loc, 1, GL_FALSE, &playerMat[0][0]);
		glm::vec4 color = projectiles[i].getColor();
		glUniform4fv(projectileshader.color, 1, &color[0]);
		projectiles[i].render();
	}
    
	glBindVertexArray(0);
	sgct::ShaderManager::instance()->unBindShaderProgram();

}

void DomeGame::printScoreboard() const {
	glm::mat4 scoreMat = MVP * glm::rotate(glm::mat4(), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::translate(glm::mat4(), glm::vec3(0.0f, 13.0f, -20.0f))
		* glm::scale(glm::mat4(), glm::vec3(1.0f));
	sgct_text::print3d(sgct_text::FontManager::instance()->getFont("Verdana", 30), sgct_text::TOP_LEFT, scoreMat, "HIGHSCORE");
	sgct_text::print3d(sgct_text::FontManager::instance()->getFont("Verdana", 14), sgct_text::TOP_LEFT, scoreMat, scoreboard.c_str());
}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("background", rootDir + "/Images/background.png", true);
	sgct::TextureManager::instance()->loadTexture("bullet", rootDir + "/Images/WEAPON.png", true);
	sgct::TextureManager::instance()->loadTexture("venus", rootDir + "/Images/venus.jpg", true);



	sgct::TextureManager::instance()->loadTexture("earth", rootDir + "/Images/earth_texture.png", true);
	sgct::TextureManager::instance()->loadTexture("lava", rootDir + "/Images/lavaplanet_texture.png", true);
	sgct::TextureManager::instance()->loadTexture("dark", rootDir + "/Images/darkplanet_texture.png", true);
	sgct::TextureManager::instance()->loadTexture("pink", rootDir + "/Images/pinkplanet_texture.png", true);
	sgct::TextureManager::instance()->loadTexture("moon", rootDir + "/Images/moontexture.png", true);


	sgct::TextureManager::instance()->loadTexture("NOBUMP", rootDir + "/Images/NOBUMP.png", true);
	sgct::TextureManager::instance()->loadTexture("NOSPEC", rootDir + "/Images/specular.png", true);
	sgct::TextureManager::instance()->loadTexture("darkpink_spec", rootDir + "/Images/specular_dark_pink.png", true);
	sgct::TextureManager::instance()->loadTexture("earthlava_spec", rootDir + "/Images/specular_earth_lava.png", true);
	sgct::TextureManager::instance()->loadTexture("testbump", rootDir + "/Images/testbump.png", true);
	sgct::TextureManager::instance()->loadTexture("spherebump", rootDir + "/Images/spherebump.png", true);
	sgct::TextureManager::instance()->loadTexture("bumpy", rootDir + "/Images/bumpy.png", true);
	sgct::TextureManager::instance()->loadTexture("projectile", rootDir + "/Images/projectile.png", true);

	sgct::TextureManager::instance()->loadTexture("dangerzone", rootDir + "/Images/dangerzone.png", true);


	
	for (int i = 0; i <= 29; i++)
		sgct::TextureManager::instance()->loadTexture("player" + std::to_string(i), rootDir + "/Images/avatar" + std::to_string(i) + ".png", true);
	sgct::TextureManager::instance()->loadTexture("playerbump", rootDir + "/Images/avatarbump2.png", true);

	sgct::TextureManager::instance()->loadTexture("weapon1", rootDir + "/Images/Weapon_1.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon1_fire", rootDir + "/Images/Weapon_1_fire.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon1normal", rootDir + "/Images/Weapon1_normalmap.png", true);

	sgct::TextureManager::instance()->loadTexture("weapon2_left", rootDir + "/Images/weapon2_left.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon2_left_fire", rootDir + "/Images/weapon2_fire_left.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon2normal_left", rootDir + "/Images/Weapon2_normalmap_left.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon2_right", rootDir + "/Images/weapon2_right.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon2_right_fire", rootDir + "/Images/weapon2_fire_right.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon2normal_right", rootDir + "/Images/Weapon2_normalmap_right.png", true);

	sgct::TextureManager::instance()->loadTexture("weapon3", rootDir + "/Images/Weapon3.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon3_fire", rootDir + "/Images/Weapon3_fire.png", true);
	sgct::TextureManager::instance()->loadTexture("weapon3normal", rootDir + "/Images/Weapon3_normalmap.png", true);

	// PlayerShader
	sgct::ShaderManager::instance()->addShaderProgram(
		"player", rootDir + "/player.vert", rootDir + "/player.frag");

	sgct::ShaderManager::instance()->bindShaderProgram("player");
	playershader.MVP_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("MVP");
	playershader.model_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("model");
	playershader.d_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("d_tex");
	playershader.b_tex_loc = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("b_tex");
	playershader.ambient = sgct::ShaderManager::instance()->getShaderProgram("player").getUniformLocation("ambient");

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

	myScene->initScene();

	DomeDrawable::initSprite();

	Weapon::init(&projectiles, &added_projectiles);
    
}

void DomeGame::addPlayer(std::string &name, std::string weaponType, glm::quat pos) {
    std::string texName = "player" + std::to_string(players.size() % 29);
    Player * newPlayer = new Player(name, texName,pos);
	
	newPlayer->setWeapon(Weapon::makeWeapon(weaponType, newPlayer), weaponType);

	std::cout << "Player " << name << " created with id: " << players.size() << " and weapon: " << weaponType << std::endl;

	players.push_back(newPlayer);
	added_players.addVal(*newPlayer);
}

void DomeGame::renderPlayer(Player *p) const {
	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS));
	glm::mat4 rot = p->getRotationMatrix();
	glm::mat4 scale = glm::scale(glm::mat4(), p->getScale()*glm::vec3(1, 1, 1));

	glm::mat4 playerMat = rot * trans * scale;
	glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &playerMat[0][0]);
	p->render();

	

}

void DomeGame::renderPlayerName(Player *p) const {
	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS));
	glm::mat4 rot = p->getRotationMatrix();
	glm::mat4 scale = glm::scale(glm::mat4(), p->getScale()*glm::vec3(1, 1, 1));

	glm::mat4 playerMat = rot * trans * scale;
	glm::mat4 nameMat = MVP * playerMat * glm::rotate(glm::translate(glm::scale(glm::mat4(), glm::vec3(0.35f)), glm::vec3(0.0f, -4.0f, -2.0f)), 0.0f * 3.14f, glm::vec3(0.0f, 0.0f, 1.0f));
	sgct_text::print3d(sgct_text::FontManager::instance()->getFont("Verdana", 16), sgct_text::TOP_CENTER, nameMat, p->playerName.c_str());
}

void DomeGame::renderWeapon(Player *p) const {
	Weapon* wp = p->getWeapon();
	float pScale = p->getScale();

	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0, 0, -(DOME_DRAWABLES_RADIUS - 0.1f)));
	glm::mat4 rot = p->getRotationMatrix() * wp->getRotationMatrix();
	glm::mat4 scale = glm::scale(glm::mat4(), pScale * wp->getScale()*glm::vec3(1, 1, 1));

	glm::mat4 weaponMat = rot * trans * scale;
	glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &weaponMat[0][0]);
	

	if (!wp->doubleWeapon) {
		wp->render();
	}
	else {
		// switch one extra time to refresh the texture in case weapon was fired

		// LEFTWEAPON
		wp->switchWeaponTexture();
		rot = p->getRotationMatrix() * wp->getSecondWeaponRotationMatrix();
		scale = glm::scale(glm::mat4(), pScale * wp->getScale()*glm::vec3(1, 1, 1));
		glm::mat4 leftWeaponMat = rot * trans * scale;
		glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &leftWeaponMat[0][0]);
		wp->render();

		// RIGHTWEAPON
		wp->switchWeaponTexture();
		rot = p->getRotationMatrix() * wp->getRotationMatrix();
		scale = glm::scale(glm::mat4(), pScale * wp->getScale()*glm::vec3(1, 1, 1));
		glm::mat4 weaponMat = rot * trans * scale;
		glUniformMatrix4fv(playershader.model_loc, 1, GL_FALSE, &weaponMat[0][0]);
		wp->render();
	}

}

void DomeGame::update(float dt) {

	myScene->update(dt);

	for (int i = 0; i < players.size(); i++) {
		//players[i]->update(dt * (1.0f - i*0.8f)); to make the second player much slower for testing
		players[i]->update(dt);
		players[i]->getWeapon()->update(dt,players[i]->c_shoot);
		glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -1.0f);
		glm::vec3 dome_dir = glm::vec3(0, sin(0.6f), -cos(0.6f));
		float dotval = glm::dot(p_pos, dome_dir);
		if (dotval < 0.6f) {
			players[i]->addWorldVelocity(dome_dir * 0.1f * (0.6f - dotval + 0.1f));
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
			glm::vec3 p1_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);
			glm::vec3 p2_pos = players[k]->getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);
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
				glm::vec3 trans = n_diff *((players[i]->getScale() + players[k]->getScale())/2.4f - glm::length(diff)) / DOME_DRAWABLES_RADIUS * 0.50f ; // 0.55f for extra margin
				players[i]->addWorldTranslation(-trans);
				players[k]->addWorldTranslation(trans);
			}
		}
	}
    
	for (int i = 0; i < players.size(); i++) {
        for(int k = 0; k < projectiles.size(); k++)
        {
			if (projectiles[k].alive()) {
				glm::vec3 p_pos = players[i]->getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);
				glm::vec3 b_pos = projectiles[k].getQuat() * glm::vec3(0, 0, -DOME_DRAWABLES_RADIUS);
				glm::vec3 diff = p_pos - b_pos;
				//If the bullet hits: decrease & increase score
				if (projectiles[k].getOwner() != players[i] && glm::length(diff) < (projectiles[k].getScale() + players[i]->getScale()) / 2.2f)
				{
					//alt knockback version
					//glm::vec3 n_diff = glm::normalize(diff);
					//glm::vec3 knockback = n_diff * glm::dot(projectiles[k].getWorldVelocity(), n_diff);

					if (players[i]->isAlive()) {
						players[i]->addWorldVelocity(projectiles[k].getWorldVelocity() * projectiles[k].knockback);
						players[i]->takeDamage(projectiles[k].damage);

						float points = projectiles[k].damage;

						//bonus points for a kill
						if (!players[i]->isAlive())
							points += 100;

						projectiles[k].getOwner()->increaseScore(points);

						projectiles[k].collide();
					}
					
					
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
    sort(playerScoreList.begin(), playerScoreList.end(), std::greater<playerScore>()); 
    
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


