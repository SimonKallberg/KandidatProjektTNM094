#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "sgct.h"

#ifndef Quad_hpp
#define Quad_hpp

class Quad {
public:
    // Constructor for objects
    Quad(const std::string& texture, float w, float h);
    Quad(const std::string& texture, float s);

    //Public member functions
	void init(); // creates Vertex array, positions and texture coords.
    void render() const;  // Handles the drawing of an object
	void bindVAO() const; // bind the vertex array object, done once before drawing all players
	void setTexture(std::string tex, std::string bump = std::string("")); // used to change the texture, used when the quad VAO is bound
    void setSize(float s); //Sets size of player
    float getSize() const; //Returns size of player
    void display() const; //Displays information of player
    
    //Public member variables
    //Texture name
    std::string textureName;
	std::string bumpTextureName;
    
private:
    // Variables
    float size;
    float width;
    float height;
	GLuint VAO = -1; // Vertex array object
	GLuint VB = -1;  // Vertex buffer
	GLuint TB = -1;  // Texture buffer
	GLuint NB = -1;  // Normal buffer
	GLuint TanB = -1;  // Tangent buffer
	GLuint BitanB = -1;  // Bitangent buffer

};

#endif
