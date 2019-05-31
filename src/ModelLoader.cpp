#include "./ModelLoader.hpp"

ModelLoader::ModelLoader(const std::string &meshName, const std::string &textureName, float scale) :
	textureName(textureName) {
	if (loadOBJ(meshName, scale, scale, scale)) {
		std::cout << "mesh loaded succesfully" << std::endl;
		initTangents();
	}
	else printf("ERROR: COULDN'T READ OBJECT\n");
}

ModelLoader::ModelLoader(const std::string &meshName, float sx, float sy, float sz) {
	if (loadOBJ(meshName, sx, sy, sz)) {
		std::cout << "simon e gud" << std::endl;
		initTangents();
	}
	else printf("ERROR: COULDN'T READ OBJECT\n");
}

ModelLoader::~ModelLoader() {
	vertices.clear();
	normals.clear();
	uvs.clear();
}

void ModelLoader::draw(float scale, glm::vec3 pos) const {
    
    // bind diffuse texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId(textureName));
    
    // bind bumpmap texture to texture slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId(bumpTextureName));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId(specularTextureName));
    
    for (unsigned int i = 0; i < vertices.size() - 2; i += 3) {
        
        glBegin(GL_TRIANGLES);
        
        //Vertex 1
        glVertexAttrib3f(4, bitangents[i].x, bitangents[i].y, bitangents[i].z);
        glVertexAttrib3f(3, tangents[i].x, tangents[i].y, tangents[i].z);
        glVertexAttrib3f(2, normals[i].x, normals[i].y, normals[i].z);
        glVertexAttrib2f(1, uvs[i].x, uvs[i].y);
        glVertexAttrib3f(0, pos.x + scale * vertices[i].x, pos.y + scale * vertices[i].y, pos.z + scale * vertices[i].z);
        
        //Vertex 2
        glVertexAttrib3f(4, bitangents[i + 1].x, bitangents[i + 1].y, bitangents[i + 1].z);
        glVertexAttrib3f(3, tangents[i + 1].x, tangents[i + 1].y, tangents[i + 1].z);
        glVertexAttrib3f(2, normals[i + 1].x, normals[i + 1].y, normals[i + 1].z);
        glVertexAttrib2f(1, uvs[i + 1].x, uvs[i + 1].y);
        glVertexAttrib3f(0, pos.x + scale * vertices[i + 1].x, pos.y + scale * vertices[i + 1].y, pos.z + scale *  vertices[i + 1].z);
        
        //Vertex 3
        glVertexAttrib3f(4, bitangents[i + 2].x, bitangents[i + 2].y, bitangents[i + 2].z);
        glVertexAttrib3f(3, tangents[i + 2].x, tangents[i + 2].y, tangents[i + 2].z);
        glVertexAttrib3f(2, normals[i + 2].x, normals[i + 2].y, normals[i + 2].z);
        glVertexAttrib2f(1, uvs[i + 2].x, uvs[i + 2].y);
        glVertexAttrib3f(0, pos.x + scale * vertices[i + 2].x, pos.y + scale * vertices[i + 2].y, pos.z + scale * vertices[i + 2].z);
        
        //std::cout << "tan: x:" << tangents[i].x << "  y: " << tangents[i].y << "  z: " << tangents[i].z << "\n";
        //std::cout << "bitan: x:" << bitangents[i].x << "  y: " << bitangents[i].y << "  z: " << bitangents[i].z << "\n";
        //std::cout << "norm: x:" << normals[i].x << "  y: " << normals[i].y << "  z: " << normals[i].z << "\n\n";
        
        glEnd();
    }
}

glm::vec3 ModelLoader::getMaxVertexValues() {
	glm::vec3 maxValues = vertices[0];
	for (int i = 1; i < vertices.size(); ++i) {
		if (vertices[i].x > maxValues.x) maxValues.x = vertices[i].x;
		if (vertices[i].y > maxValues.y) maxValues.y = vertices[i].y;
		if (vertices[i].z > maxValues.z) maxValues.z = vertices[i].z;
	}
	return maxValues;
}

glm::vec3 ModelLoader::getMinVertexValues() {
	glm::vec3 minValues = vertices[0];
	for (int i = 1; i < vertices.size(); ++i) {
		if (vertices[i].x < minValues.x) minValues.x = vertices[i].x;
		if (vertices[i].y < minValues.y) minValues.y = vertices[i].y;
		if (vertices[i].z < minValues.z) minValues.z = vertices[i].z;
	}
	return minValues;
}

void ModelLoader::initTangents() {
	for (unsigned int i = 0; i < vertices.size() - 2; i += 3) {
		glm::vec3 edge1 = vertices[i + 1] - vertices[i];
		glm::vec3 edge2 = vertices[i + 2] - vertices[i];
		glm::vec2 deltaUV1 = uvs[i + 1] - uvs[i];
		glm::vec2 deltaUV2 = uvs[i + 2] - uvs[i];

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);

		glm::vec3 T;
		glm::vec3 B;

		T = glm::normalize(tangent - glm::dot(tangent, normals[i]) * normals[i]);
		B = glm::cross(normals[i], T);
		tangents.push_back(T);
		bitangents.push_back(B);

		T = glm::normalize(tangent - glm::dot(tangent, normals[i]) * normals[i + 1]);
		B = glm::cross(normals[i + 1], T);
		tangents.push_back(T);
		bitangents.push_back(B);

		T = glm::normalize(tangent - glm::dot(tangent, normals[i]) * normals[i + 2]);
		B = glm::cross(normals[i + 2], T);
		tangents.push_back(T);
		bitangents.push_back(B);

	}
	std::cout << "TANGENT INIT\n";
}

bool ModelLoader::loadOBJ(const std::string &meshName, float sx, float sy, float sz) {
	printf("Loading mesh %s... ", meshName.c_str());

	std::string builder = "";
	//builder += "obj/";
	builder += meshName;
	builder += ".obj";
	const char* path = builder.c_str();

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;



	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Resize
		vertex.x *= sx;
		vertex.y *= sy;
		vertex.z *= sz;

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);

	}
	printf("verts: %i\n", (int)vertices.size());
	printf("normals: %i\n", (int)normals.size());
	printf("uvs: %i\n", (int)uvs.size());
	return true;
}
