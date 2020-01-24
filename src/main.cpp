#include "./sgct.h"
#include "../sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <mmsystem.h>
#include "./ServerHandler.hpp"
//#include "./boxtest.hpp"
#include "./Quad.hpp"
#include "./Scene.hpp"
#include "./Player.hpp"
#include "./DomeGame.hpp"
#include "./ModelLoader.hpp"

sgct::Engine * gEngine;
DomeGame * domeGame;

//boxtest * box;
//sgct::SharedObject<boxtest> s_box;

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
	if (msgType == 'C') // controls were sent for one player, structure: CIBV, for: [ *CONTROLS*, playerindex, button, value ]
	{
		int playerIndex;
		strm >> playerIndex;
		char control;
		strm >> control; // L, R
		int value;
		strm >> value; // 1 0, on off
		//std::cout << "INDEX:" << playerIndex;
		//std::cout << " CONTROL:" << control;
		//std::cout << " VALUE:" << value << "\n";
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
	else if (msgType == 'P') // player added P 
	{
		std::string name;
		std::string weapon;
		strm >> name;
		strm >> weapon;

		float angle = ((float)rand() / RAND_MAX) * 6.28f;
		float dist_from_center = ((float)rand() / RAND_MAX) * 0.4f + 0.2f;
		
		glm::quat randPos = glm::quat();
		randPos *= glm::quat(glm::vec3(0.6f, 0.0f, 0.0f));
		randPos *= glm::quat(glm::vec3(0.0f, 0.0f, angle));
		randPos *= glm::quat(glm::vec3(-dist_from_center, 0.0f, 0.0f));
		randPos = glm::normalize(randPos);

		domeGame->addPlayer(name, weapon, randPos);
	}
	else if (msgType == 'U') // Update Weapon 
	{
		int id;
		std::string weapon;
		strm >> id;
		strm >> weapon;
		domeGame->players[id]->setWeapon(Weapon::makeWeapon(weapon, domeGame->players[id]), weapon);

		DomeGame::weaponInfo wp_inf;
		wp_inf.id = id;
		wp_inf.weaponType = weapon;
		domeGame->changed_weapons.addVal(wp_inf);
	}
}

//Ensures compability for file paths on both mac and windows
std::string findRootDir(std::string rootDir)
{
    std::ifstream ifs;
    ifs.open(rootDir + "/config.h");
    std::cout << rootDir << std::endl;
    
    while( (ifs.rdstate() & std::ifstream::failbit ) != 0 )
    {
        rootDir = rootDir.substr(0, rootDir.find_last_of("\\/"));
        ifs.open(rootDir + "/config.h");
        std::cout << rootDir << std::endl;
        if(rootDir == "")
        {
            std::cout << "Couldn't find root directory!" << std::endl;
            break;
        }
        
    }
    return rootDir;
}

int main(int argc, char* argv[])
{
    //Find root directory for the project
    std::string rootDir = findRootDir(argv[0]);
    
    // Allocate
    gEngine = new sgct::Engine(argc, argv);
	domeGame = new DomeGame(gEngine, rootDir);

	//box = new boxtest();

    // Bind your functions
	gEngine->setInitOGLFunction(myInitOGLFun);
    gEngine->setDrawFunction(myDrawFun);
    gEngine->setPreSyncFunction(myPreSyncFun);
    gEngine->setKeyboardCallbackFunction(keyCallback);
    sgct::SharedData::instance()->setEncodeFunction(myEncodeFun);
    sgct::SharedData::instance()->setDecodeFunction(myDecodeFun);
    
    // Init the engine
    if (!gEngine->init(sgct::Engine::OpenGL_Compablity_Profile))
    {
        delete gEngine;
        return EXIT_FAILURE;
    }
    
    // Main loop
    gEngine->render();
    
    // Clean up (de-allocate)
    delete gEngine;
	delete domeGame;

    // Exit program
    exit(EXIT_SUCCESS);
}


void myInitOGLFun() {
    std::cout << "Init started.." << std::endl;
    //Add Verdana size 14 to the FontManager using the system font path
    if( !sgct_text::FontManager::instance()->addFont( "Verdana", "verdana.ttf" ) )
        sgct_text::FontManager::instance()->getFont( "Verdana", 14 );
    
    domeGame->init();
    std::cout << "Init DONE!" << std::endl;
	curr_time.setVal(sgct::Engine::getTime());

	if (gEngine->isMaster()) {
		std::cout << "Master node attemping connection to server.\n";;
		ServerHandler::setMessageCallback(getServerMsg);
		ServerHandler::connect();

		PlaySound("../DOMEMUSICtest.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	}
	//boxtest::init();
	
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
		ServerHandler::service();
		domeGame->update(delta_time);
		
    }
}

void keyCallback(int key, int action)
{
    std::string playerName = "DOME_MASTER";
    if( gEngine->isMaster() )
    {
        switch( key )
        {
            case 'A':
					if(action == SGCT_PRESS)
						domeGame->players[0]->c_left = 1;
					if (action == SGCT_RELEASE)
						domeGame->players[0]->c_left = 0;
					//box->Box_x -= 0.2f;
                break;
            case 'D':
                if(action == SGCT_PRESS)
                    domeGame->players[0]->c_right = 1;
                if (action == SGCT_RELEASE)
                    domeGame->players[0]->c_right = 0;
					//box->Box_x += 0.2f;
                break;
            case 'W':
                    //domeGame->players[0]->setPosition(0.0f, STEPLENGTH);
					//box->Box_z -= 0.2f;
				break;
            case 'S':
                    //domeGame->players[0]->setPosition(0.0f, -STEPLENGTH);
					//box->Box_z += 0.2f;
                break;
			case SGCT_KEY_SPACE:
					//box->Box_y += 0.2f;
				break;
			case SGCT_KEY_LCTRL:
					//box->Box_y -= 0.2f;
				break;
			case 'Z':
					//box->Box_scale += 0.1f;
					if (action == SGCT_PRESS)
						domeGame->addPlayer(playerName, "rifle");
				break;
			case 'X':
					//box->Box_scale -= 0.1f;
					if (action == SGCT_PRESS)
						domeGame->addPlayer(playerName, "shotgun");
				break;
			case 'C':
					if (action == SGCT_PRESS)
						domeGame->addPlayer(playerName, "light");
				break;
			case 'V':
				if (action == SGCT_PRESS)
					domeGame->addPlayer(playerName, "popgun");
				break;
			case 'L':
					//std::cout << "X: " << box->Box_x << " Y: " << box->Box_y << " Z: " << box->Box_z << " SCALE: " << //box->Box_scale << "\n";
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
    sgct::SharedString s_score = domeGame->scoreboard;
    sgct::SharedData::instance()->writeString(&s_score);
    
	sgct::SharedData::instance()->writeVector(&domeGame->added_players);
	domeGame->added_players.clear();

	sgct::SharedData::instance()->writeVector(&domeGame->added_projectiles);
	domeGame->added_projectiles.clear();

	sgct::SharedData::instance()->writeVector(&domeGame->removed_projectiles);
	domeGame->removed_projectiles.clear();

	sgct::SharedData::instance()->writeVector(&domeGame->changed_weapons);
	domeGame->changed_weapons.clear();

	for (int i = 0; i < domeGame->players.size(); i++) {
		domeGame->players[i]->writeData();
		domeGame->players[i]->getWeapon()->writeData();
	}

	for (int i = 0; i < domeGame->projectiles.size(); i++) {
		domeGame->projectiles[i].writeData();
	}

	domeGame->myScene->writeData();
}


void myDecodeFun()
{
    sgct::SharedString s_score;
    sgct::SharedData::instance()->readString(&s_score);
    
    domeGame->scoreboard = s_score.getVal();
    
	//std::cout << "\n0 ADDPLAYER:\n";

	sgct::SharedData::instance()->readVector(&domeGame->added_players);
	std::vector<Player> add_players = domeGame->added_players.getVal();
	for (int i = 0; i < add_players.size(); i++) {
		//std::cout << "0";
        std::string player_name = add_players[i].getName();
		domeGame->addPlayer(player_name, add_players[i].weaponType, add_players[i].getQuat());
	}

	//std::cout << "\n0-1 ADDPROJECTILES:\n";
	sgct::SharedData::instance()->readVector(&domeGame->added_projectiles);
	std::vector<Projectile> add_projectiles = domeGame->added_projectiles.getVal();
	for (int i = 0; i < add_projectiles.size(); i++) {
		//std::cout << "1";
		domeGame->projectiles.push_back(Projectile(add_projectiles[i]));

	}

	//std::cout << "\n0-2 REMPROJECTILES:\n";
	sgct::SharedData::instance()->readVector(&domeGame->removed_projectiles);
	std::vector<int> rem_indices = domeGame->removed_projectiles.getVal();
	for (int i = 0; i < rem_indices.size(); i++) {
		//std::cout << "2";
		domeGame->projectiles.erase(domeGame->projectiles.begin() + rem_indices[i]);
	}

	sgct::SharedData::instance()->readVector(&domeGame->changed_weapons);
	std::vector<DomeGame::weaponInfo> changed = domeGame->changed_weapons.getVal();
	for (int i = 0; i < changed.size(); i++) {
		domeGame->players[changed[i].id]->setWeapon(Weapon::makeWeapon(changed[i].weaponType, domeGame->players[changed[i].id]), changed[i].weaponType);
	}
		
	//std::cout << "\n0-3 READPLAYERS:\n";

	for (int i = 0; i < domeGame->players.size(); i++) {
		//std::cout << "3";

		domeGame->players[i]->readData();
		domeGame->players[i]->getWeapon()->readData();
	}


	//std::cout << "\n0-4 READPROJECTILES\n";
	for (int i = 0; i < domeGame->projectiles.size(); i++) {
		//std::cout << "4";

		domeGame->projectiles[i].readData();
	}
	
	domeGame->myScene->readData();
}
