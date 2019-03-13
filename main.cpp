#include "./sgct.h"
#include "./sgct/include/sgct/Engine.h"
#include <iostream>
#include <math.h>
#include "sgct.h"
#include "player.hpp"

sgct::Engine * gEngine;

void myDrawFun();
void myPreSyncFun();
void myEncodeFun();
void myDecodeFun();
void keyCallback(int key, int action);
void renderPlayer();

sgct::SharedDouble curr_time(0.0);

float speed = 0.0f;
const float RADIUS = 7.4f;
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

    renderPlayer();
    
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
                Buttons[UP] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
                    test.setPosition(STEPLENGTH, 0.0f);
                std::cout << STEPLENGTH << std::endl;
                break;
            case SGCT_KEY_S:
                if(action == SGCT_PRESS)
                    test.setPosition(-STEPLENGTH, 0.0f);
                break;
            case SGCT_KEY_W:
                if(action == SGCT_PRESS)
                    test.setPosition(0.0f, STEPLENGTH);
                break;
            case SGCT_KEY_Z:
                if(action == SGCT_PRESS)
                    test.setPosition(0.0f, -STEPLENGTH);
                break;
        }
    }
}

void renderPlayer(){
    
    //Converting spherical coordinates to cartesian coordinates
    float x = RADIUS*sin(test.theta)*cos(test.phi);
    float y = RADIUS*sin(test.theta)*sin(test.phi);
    float z = RADIUS*cos(test.theta);

    //render a single triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); //Red
    glVertex3f(x-0.5f, y-0.5f, z+0.0f);
    
    glColor3f(0.0f, 1.0f, 0.0f); //Green
    glVertex3f(x+0.0f, y+0.5f, z+1.0f);
    
    glColor3f(0.0f, 0.0f, 1.0f); //Blue
    glVertex3f(x+0.5f, y-0.5f, z+0.0f);
    glEnd();
}
