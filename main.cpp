
#include <iostream>

#include "sgct/include/sgct/Engine.h"


sgct::Engine * gEngine;

void myDrawFun();

int main( int argc, char* argv[] ){
    
    std::cout<<"Hello world"<< std::endl;
    
    // Allocate
    /*
    gEngine = new sgct::Engine( argc, argv );
    
    
    // Bind your functions
    gEngine->setDrawFunction( myDrawFun );
    
    // Init the engine
    if( !gEngine->init() )
    {
        delete gEngine;
        return EXIT_FAILURE;
    }
    
    // Main loop
    gEngine->render();
    
    // Clean up (de-allocate)
    delete gEngine;
    
    // Exit program
    exit( EXIT_SUCCESS );*/
}

void myDrawFun()
{
    //render a single triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); //Red
    glVertex3f(-0.5f, -0.5f, 0.0f);
    
    glColor3f(0.0f, 1.0f, 0.0f); //Green
    glVertex3f(0.0f, 0.5f, 0.0f);
    
    glColor3f(0.0f, 0.0f, 1.0f); //Blue
    glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();
}
