#include "./domegame.hpp"

DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};

void DomeGame::render() const{
	myScene->render();
    for (int i = 0; i < players.size(); i++){
        players[i]->render();
    }
    for (int i = 0; i < bullets.size(); i++){
        bullets[i]->render();
    }
}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("background", "../../Images/background.png", true);
	sgct::TextureManager::instance()->loadTexture("player", "../../Images/player.png", true);
	sgct::TextureManager::instance()->loadTexture("bullet", "../../Images/bullet.jpg", true);
	sgct::TextureManager::instance()->loadTexture("venus", "../../Images/venus.jpg", true);
    
    std::string avatar = "avatar";
    
    for(int i = 1; i < 31; i++)
    {
        sgct::TextureManager::instance()->loadTexture(avatar + std::to_string(i), "../../Images/" + avatar + std::to_string(i) + ".png", true);
    }

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
    
    std::string test1 = "Ylva1";
    std::string test2 = "Ylva2";
    
    for(int i = 0; i < 10; i++)
    {
        float r2 = -90 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(180)));
        addPlayer(test1, r2, r2, 0);
    }
}

void DomeGame::addPlayer(std::string &name) {
    std::string texName = "avatar" +  std::to_string(players.size() + 1);
    Player * newPlayer = new Player(name, texName, &bullets);
    players.push_back(newPlayer);
    std::cout << "Player created" << std::endl;
}

void DomeGame::addPlayer(std::string &name, float in_theta, float in_phi, float in_dir ) {
    std::string texName = "avatar" +  std::to_string(players.size() + 1);
    Player * newPlayer = new Player(in_theta, in_phi, in_dir, name, texName, &bullets);
    players.push_back(newPlayer);
    std::cout << "Player created" << std::endl;
}

void DomeGame::update() {
    float sizeOfBullet = 4.0f;
    
	for (int i = 0; i < players.size(); i++) {
		players[i]->update();
        for(int k = 0; k < bullets.size(); k++)
        {
            //If the bullet hits, decrease & increase score
            if(bullets[k]->getPhi() < players[i]->getPhi() + sizeOfBullet &&
               bullets[k]->getPhi() > players[i]->getPhi() - sizeOfBullet &&
               bullets[k]->getTheta() < players[i]->getTheta() + sizeOfBullet &&
               bullets[k]->getTheta() > players[i]->getTheta() - sizeOfBullet &&
               bullets[k]->getOwner() != players[i] )
            {
                players[i]->decreaseScore();
                bullets[k]->getOwner()->increaseScore();
                bullets.erase(bullets.begin() + k);
            }
        }
	}
    for (int i = 0; i < bullets.size(); i++) {
        
        if(!bullets[i]->update())
        {
            bullets.erase(bullets.begin() + i);
        }
    }
}
