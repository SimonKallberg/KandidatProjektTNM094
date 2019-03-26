const float RADIUS = 7.4f;

#include "./sgct.h"
#include "./sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.hpp"
#include <libwebsockets.h>
#include "Quad.hpp"
#include "Scene.hpp"

sgct::Engine * gEngine;

void myDrawFun();
void myPreSyncFun();
void myEncodeFun();
void myDecodeFun();
void keyCallback(int key, int action);

Scene *testScene;


sgct::SharedDouble curr_time(0.0);

float speed = 0.0f;

float STEPLENGTH = 0.9f;
Player test = Player();

int main(int argc, char* argv[])
{
    
    // Allocate
    gEngine = new sgct::Engine(argc, argv);
    
    // Bind your functions
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

void myDrawFun()
{
    glRotatef(static_cast<float>(curr_time.getVal()) * speed, 0.0f, 1.0f, 0.0f);

    test.render();
    testScene->draw();
    
}

void myPreSyncFun()
{
    //set the time only on the master
    if (gEngine->isMaster())
    {
        //get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());
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
                    test.setPosition(STEPLENGTH, 0.0f);

                break;
            case 'S':
                    test.setPosition(-STEPLENGTH, 0.0f);
                break;
            case 'W':
                    test.setPosition(0.0f, STEPLENGTH);
            case 'Z':
                    test.setPosition(0.0f, -STEPLENGTH);
                break;
        }
    }
}
