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
    void draw(float x=0.0f, float y=0.0f, float z=0.0f) const;
    
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
