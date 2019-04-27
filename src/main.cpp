#include "./sgct.h"
#include "../sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "./ServerHandler.hpp"
#include "./boxtest.hpp"
#include "./Quad.hpp"
#include "./Scene.hpp"
//#include "./Bullet.hpp"
#include "./Player.hpp"
#include "./DomeGame.hpp"
#include "./ModelLoader.hpp"

sgct::Engine * gEngine;
DomeGame * domeGame;

boxtest * box;
sgct::SharedObject<boxtest> s_box;

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
	char msgType = 'N';
	strm >> msgType;
	if (msgType == 'P') // player added
	{
		std::string name;
		strm >> name;
		std::cout << "Player " + name + " added:\n";
		domeGame->addPlayer(name);
	}
	else if (msgType == 'C') // controls were sent for one player, structure: CIBV, for: [ *CONTROLS*, playerindex, button, value ]
	{
		int playerIndex;
		strm >> playerIndex;
		char control;
		strm >> control; // L, R
		int value;
		strm >> value; // 1 0, on off
		std::cout << "INDEX:" << playerIndex;
		std::cout << " CONTROL:" << control;
		std::cout << " VALUE:" << value << "\n";
		switch (control)
		{
			case 'L':
				domeGame->players[playerIndex]->c_left = value;
				break;
			case 'R':
				domeGame->players[playerIndex]->c_right = value;
				break;
			case 'F':
				domeGame->players[playerIndex]->c_shoot = value;
				break;
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
    std::cout << "Init DONE!" << std::endl;
	curr_time.setVal(sgct::Engine::getTime());

	boxtest::init();
}


void myDrawFun()
{
	domeGame->MVP = gEngine->getCurrentModelViewProjectionMatrix();
	domeGame->render();
	//s_box.getVal().draw();

}

void myPreSyncFun()
{
    //set the time only on the master
    if (gEngine->isMaster())
    {
		float delta_time = curr_time.getVal();
        //get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());
		delta_time = curr_time.getVal() - delta_time;

		//std::cout << 1/delta_time << "FPS\n";
		//ServerHandler::service();
		domeGame->update(delta_time);
		
    }
}

void keyCallback(int key, int action)
{
    if( gEngine->isMaster() )
    {
        switch( key )
        {
            case 'A':
					if(action == SGCT_PRESS)
						domeGame->players[0]->c_left = 1;
					if (action == SGCT_RELEASE)
						domeGame->players[0]->c_left = 0;
					box->Box_x -= 0.2f;
                break;
            case 'D':
                if(action == SGCT_PRESS)
                    domeGame->players[0]->c_right = 1;
                if (action == SGCT_RELEASE)
                    domeGame->players[0]->c_right = 0;
					box->Box_x += 0.2f;
                break;
            case 'W':
                    //domeGame->players[0]->setPosition(0.0f, STEPLENGTH);
					box->Box_z -= 0.2f;
				break;
            case 'S':
                    //domeGame->players[0]->setPosition(0.0f, -STEPLENGTH);
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
					if (action == SGCT_PRESS)
						domeGame->addPlayer(std::string("DOME_MASTER"));
				break;
			case 'L':
					std::cout << "X: " << box->Box_x << " Y: " << box->Box_y << " Z: " << box->Box_z << " SCALE: " << box->Box_scale << "\n";
				break;
            case 'K':
				if (action == SGCT_PRESS)
					domeGame->players[0]->c_shoot = 1;
				if (action == SGCT_RELEASE)
					domeGame->players[0]->c_shoot = 0;
                break;
        }
    }
}


void myEncodeFun()
{
	sgct::SharedData::instance()->writeDouble(&curr_time);
	s_box.setVal(*box);
	sgct::SharedData::instance()->writeObj(&s_box);
}


void myDecodeFun()
{
	sgct::SharedData::instance()->readDouble(&curr_time);
	sgct::SharedData::instance()->readObj(&s_box);
}