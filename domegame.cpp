//#include "Player.hpp"
#include "./Scene.hpp"

#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
	// Constructor
	DomeGame(sgct::Engine * gEngine);

	void init();
	void draw();

	Scene * myScene;
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