#ifndef Quad_hpp
#define Quad_hpp

#include <stdio.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "sgct.h"

class Quad {
public:
	// Constructor for objects
	Quad(const std::string& texture, float w, float h);
	Quad(const std::string& texture, float s);

	// Handles the drawing of an object
	void draw(float x = 0.0f, float y = 0.0f, float z = 0.0f) const;

	//Sets size of player
	void setSize(float s);

	//Returns size of player
	float getSize() const;

	//Displays information of player
	void display() const;

	//Texture name
	std::string textureName;

private:
	// Variables
	float size;
	float width;
	float height;
};

#endif


// Quad constructor, using width and heigth
Quad::Quad(const std::string& t, float w, float h):
textureName(t), width(w), height(h) {
    std::cout << "Quad constructor" << std::endl;
    size = fmax(w,h);
}

// Quad constructor, using only size
Quad::Quad(const std::string& t, float s):
textureName(t), size(s) {
    std::cout << "Quad constructor" << std::endl;
    width = s;
    height = s;
}

// Draw player based on position and MVP matrix
void Quad::draw(float x, float y, float z) const {
    // Set the active texture unit
    glActiveTexture(GL_TEXTURE0);
    
    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));
    
    glPushMatrix();
    glTranslatef(x,y,z);
    
    // Draw the player polygon
    glBegin(GL_QUADS);
    // Set the normal of the polygon
    glNormal3f(0.0, 0.0, 1.0);
    
    // Set starting position of the texture mapping
    // The polygon is drawn from the world coordinates perspective
    // (we set the origin in the center of the polygon)
    // while the texture is drawn from the polygons coordinates
    // (we draw from the bottom-left corner of the polygon)
    
    // Define polygon vertices in counter clock wise order
    glTexCoord2d(1, 0);
    glVertex3f(+width, -height, 0);
    
    glTexCoord2d(1, 1);
    glVertex3f(+width, +height, 0);
    
    glTexCoord2d(0, 1);
    glVertex3f(-width, +height, 0);
    
    glTexCoord2d(0, 0);
    glVertex3f(-width, -height, 0);
    
    glEnd();
    
    glPopMatrix();
    
}

// Set size of player
void Quad::setSize(float s) {
    size = s;
}

// Get size of player
float Quad::getSize() const {
    return size;
}

// Displays info about a Quad
void Quad::display() const{
    std::cout << "size = " << size << std::endl;
    std::cout << "width = " << width << std::endl;
    std::cout << "height = " << height << std::endl;
    std::cout << "textureName = " << textureName << std::endl;
}