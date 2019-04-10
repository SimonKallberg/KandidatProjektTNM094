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
	sgct::TextureManager::instance()->loadTexture("background", "../Images/background.png", true);
	sgct::TextureManager::instance()->loadTexture("player", "../Images/player.png", true);
	sgct::TextureManager::instance()->loadTexture("bullet", "../Images/bullet.jpg", true);

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
	/*std::string apa = "apa";
	addPlayer(apa);*/
}

void DomeGame::addPlayer(std::string &name) {
    Player * newPlayer = new Player(name, &bullets);
    players.push_back(newPlayer);
}

void DomeGame::update() {
    float sizeOfBullet = 0.1f;
    
	for (int i = 0; i < players.size(); i++) {
		players[i]->update();
        for(int k = 0; k < bullets.size(); k++)
        {
            //If the bullet hits, decrease score
            if(bullets[k]->getPhi() < players[i]->getPhi() + sizeOfBullet &&
               bullets[k]->getPhi() > players[i]->getPhi() - sizeOfBullet &&
               bullets[k]->getTheta() < players[i]->getTheta() + sizeOfBullet &&
               bullets[k]->getTheta() > players[i]->getTheta() - sizeOfBullet)
            {
                players[i]->decreaseScore();
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
