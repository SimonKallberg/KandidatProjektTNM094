//#include "Player.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
	// Constructor
	DomeGame(sgct::Engine * gEngine);

	void init();
	void draw() const;
	size_t textureHandle;
	Scene * myScene;
private:
	
};

#endif // DOMEGAME

DomeGame::DomeGame(sgct::Engine * gEngine) {		//Constructor
	myScene = new Scene();
	std::cout << "DomeGame gjord" << std::endl;
};

void DomeGame::draw() const{
	myScene->draw();
}

void DomeGame::init() {
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);	
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexture("avatar3", "./avatar3.png", true);

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