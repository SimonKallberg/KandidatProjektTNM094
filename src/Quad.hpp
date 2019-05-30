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

	// creates Vertex array, positions and texture coords.
	void init();
    
    // Handles the drawing of an object
    void render() const;

	// bind the vertex array object, done once before drawing all players
	void bindVAO() const;

	// used to change the texture, used when the quad VAO is bound
	void setTexture(std::string tex, std::string bump = std::string(""));
    
    //Sets size of player
    void setSize(float s);
    
    //Returns size of player
    float getSize() const;
    
    //Displays information of player
    void display() const;
    
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
