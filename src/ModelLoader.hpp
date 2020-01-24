#include <vector> 
#include <string>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include <sgct.h>

#ifndef MODELLOADER_H
#define MODELLOADER_H

class ModelLoader{
public:
    
    //Constructor
	ModelLoader(const std::string &meshName, const std::string &textureName, float scale = 1.0f);
	ModelLoader(const std::string &meshName, float sx, float sy, float sz);
    
    //Destructor
	~ModelLoader();

    //Public member functions
    void draw(float scale = 1, glm::vec3 pos = glm::vec3(0, 0, 0)) const;
	glm::vec3 getMaxVertexValues();
	glm::vec3 getMinVertexValues();

    //Public member variables
    //Vertices, normals, uvs
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	// Tangent space stuff for bumpmaps
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
    //Textures
	std::string textureName;
	std::string bumpTextureName = "NOBUMP";
	std::string specularTextureName = "NOSPEC";

private:

	// calculate the tangent space vectors
	void initTangents();
	bool loadOBJ(const std::string &meshName, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);

};

#endif
