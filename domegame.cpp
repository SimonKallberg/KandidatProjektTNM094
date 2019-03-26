//#include "Player.hpp"
//#include "scene.h"

#ifndef DOMEGAME
#define DOMEGAME


class DomeGame{
public:
	// Constructor
	DomeGame(sgct::Engine * gEngine);

	void init();
	void draw();
private:
	
};

#endif // DOMEGAME

DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};


void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	//sgct::TextureManager::instance()->loadTexure(trash, "", "", true); //Texture
}