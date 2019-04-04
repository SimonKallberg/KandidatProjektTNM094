#include "./sgct.h"
#include "../sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "./ServerHandler.hpp"
#include "./boxtest.hpp"
#include "./Quad.hpp"
#include "./Scene.hpp"
#include "./Player.hpp"
#include "./DomeGame.hpp"
#include "./ModelLoader.hpp"


sgct::Engine * gEngine;
DomeGame * domeGame;

boxtest * box;

void myDrawFun();
void myPreSyncFun();
void myEncodeFun();
void myDecodeFun();
void keyCallback(int key, int action);
void myInitOGLFun();


sgct::SharedDouble curr_time(0.0);

float speed = 0.0f;

float STEPLENGTH = 0.1f;

void getServerMsg(const char * msg, size_t len)
{
	std::istringstream strm(msg);
	char msgType;
	strm >> msgType;
	if (msgType == 'P') // player added
	{
		std::cout << "PLAYER ADDED\n";
	}
	else if (msgType == 'C') // controls were sent for one player
	{
		std::cout << "controls received:\n";
		int playerIndex = -1;
		strm >> playerIndex;
		int turn = 0;
		strm >> turn;
		std::cout << "TURN:" << turn << "\n";
		domeGame->players[playerIndex]->setControls(turn);
	}
}


int main(int argc, char* argv[])
{
	ServerHandler::setMessageCallback(getServerMsg);
	ServerHandler::connect();

    // Allocate
    gEngine = new sgct::Engine(argc, argv);
	domeGame = new DomeGame(gEngine);
    
    Player * test = new Player();
    Player * test1 = new Player();
    
    domeGame->addPlayer(test);
    domeGame->addPlayer(test1);

	box = new boxtest();

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
    sgct::TextureManager::instance()->loadTexture("player", "../player.png", true);
    std::cout << "Init DONE!" << std::endl;
}

void myDrawFun()
{
    glRotatef(static_cast<float>(curr_time.getVal()) * speed, 0.0f, 1.0f, 0.0f);
	domeGame->draw();
	box->draw();
}

void myPreSyncFun()
{
    //set the time only on the master
    if (gEngine->isMaster())
    {
        //get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());

		ServerHandler::service();
		domeGame->updatePlayers();
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
					if(action == SGCT_PRESS)
						domeGame->players[0]->setControls(-1);
					if (action == SGCT_RELEASE)
						domeGame->players[0]->setControls(0);
					box->Box_x -= 0.2f;
                break;
            case 'D':
				if (action == SGCT_PRESS)
					domeGame->players[0]->setControls(1);
				if (action == SGCT_RELEASE)
					domeGame->players[0]->setControls(0);
					box->Box_x += 0.2f;
                break;
            case 'W':
                    domeGame->players[0]->setPosition(0.0f, STEPLENGTH);
					box->Box_z -= 0.2f;
				break;
            case 'S':
                    domeGame->players[0]->setPosition(0.0f, -STEPLENGTH);
					box->Box_z += 0.2f;
                break;
			case SGCT_KEY_SPACE:
					box->Box_y += 0.2f;
				break;
			case SGCT_KEY_LCTRL:
					box->Box_y -= 0.2f;
				break;
			case 'Z':
					box->Box_scale += 0.1f;
				break;
			case 'X':
					box->Box_scale -= 0.1f;
				break;
			case 'L':
				std::cout << "X: " << box->Box_x << " Y: " << box->Box_y << " Z: " << box->Box_z << " SCALE: " << box->Box_scale << "\n";
				break;
        }
    }
}



