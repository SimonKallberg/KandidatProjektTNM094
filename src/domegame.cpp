#include "./DomeGame.hpp"


DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};

void DomeGame::draw() const{
	myScene->draw();
    for (int i = 0; i < players.size(); i++){
        players[i]->render();
    }
}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("background", "../background.png", true);

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
	glDisable(GL_DEPTH_TEST);

	std::cout << "johan was here xD" << std::endl;
}

void DomeGame::addPlayer(Player * newPlayer) {
    players.push_back(newPlayer);
}

void DomeGame::renderPlayer(Player * newPlayer) {
    newPlayer->render();
}
