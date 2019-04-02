#include "./sgct.h"
#include "../sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <libwebsockets.h>
#include "./ServerHandler.hpp"
#include "./Quad.hpp"
#include "./Scene.hpp"
#include "./Player.hpp"
#include "./DomeGame.hpp"
#include "./ModelLoader.hpp"


sgct::Engine * gEngine;
DomeGame * domeGame;
Player * test;

void myDrawFun();
void myPreSyncFun();
void myEncodeFun();
void myDecodeFun();
void keyCallback(int key, int action);
void myInitOGLFun();


sgct::SharedDouble curr_time(0.0);

float speed = 0.0f;

float STEPLENGTH = 0.9f;

void getServerMsg(const char * msg, size_t len)
{
	std::istringstream strm(msg);
	char msgType;
	strm >> msgType;
	if (msgType == 'P') // player added
	{
		std::cout << "PLAYER ADDED\n";
	}
	else if (msgType == 'C') // controls were sent  (currently setup for player_amount x controls_size, as in one C message sends all players controls)
	{
		std::cout << "controls received:\n";
		int playerIndex = 0;
		while (true) {
			float controls[4];
			for (int i = 0; i < 4; i++) {
				strm >> controls[i];
			}
			if (!strm)
				break;

			std::cout << controls[0] << ", " << controls[1] << ", " << controls[2] << ", " << controls[3] << "\n";
			// handle controls in controls[];
		}
	}
}


int main(int argc, char* argv[])
{
	ServerHandler::setMessageCallback(getServerMsg);
	ServerHandler::connect();

    // Allocate
    gEngine = new sgct::Engine(argc, argv);
	domeGame = new DomeGame(gEngine);
    test = new Player();

    // Bind your functions
	gEngine->setInitOGLFunction(myInitOGLFun);
    gEngine->setDrawFunction(myDrawFun);
    gEngine->setPreSyncFunction(myPreSyncFun);
    gEngine->setKeyboardCallbackFunction(keyCallback);
    sgct::SharedData::instance()->setEncodeFunction(myEncodeFun);
    sgct::SharedData::instance()->setDecodeFunction(myDecodeFun);
    
    // Init the engine
    if (!gEngine->init())
    {
        delete gEngine;
        return EXIT_FAILURE;
    }
    
    // Main loop
    gEngine->render();
    
    // Clean up (de-allocate)
    delete gEngine;
    
    // Exit program
    exit(EXIT_SUCCESS);
}

void myInitOGLFun() {
    std::cout << "Init started.." << std::endl;
    domeGame->init();
    sgct::TextureManager::instance()->loadTexture("player", "player.png", true);
    std::cout << "Init DONE!" << std::endl;
}

void myDrawFun()
{
    glRotatef(static_cast<float>(curr_time.getVal()) * speed, 0.0f, 1.0f, 0.0f);
	domeGame->draw();
    test->render();
}

void myPreSyncFun()
{
    //set the time only on the master
    if (gEngine->isMaster())
    {
        //get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());

		ServerHandler::service();
    }
}

void myEncodeFun()
{
    sgct::SharedData::instance()->writeDouble(&curr_time);
}

void myDecodeFun()
{
    sgct::SharedData::instance()->readDouble(&curr_time);
}

void keyCallback(int key, int action)
{
    if( gEngine->isMaster() )
    {
        switch( key )
        {
            case 'A':
                    test->setPosition(STEPLENGTH, 0.0f);

                break;
            case 'S':
                    test->setPosition(-STEPLENGTH, 0.0f);
                break;
            case 'W':
                    test->setPosition(0.0f, STEPLENGTH);
				break;
            case 'Z':
                    test->setPosition(0.0f, -STEPLENGTH);
                break;
        }
    }
}



