#include "./Quad.hpp"

// Quad constructor, using width and heigth
Quad::Quad(const std::string& t, float w, float h):
textureName(t), width(w), height(h) {
    std::cout << "Quad constructor" << std::endl;
    size = fmax(w,h);
	init();
}

// Quad constructor, using only size
Quad::Quad(const std::string& t, float s):
textureName(t), size(s) {
    std::cout << "Quad constructor" << std::endl;
    width = s;
    height = s;
	init();
}

void Quad::init() {
	const GLfloat vertex[] = {
		-width / 2.0f, height / 2.0f, 0.0f,
		-width / 2.0f, -height / 2.0f, 0.0f,
		width / 2.0f, height / 2.0f, 0.0f,
		width / 2.0f, -height / 2.0f, 0.0f

	};

	const GLfloat texture[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,3, GL_FLOAT, GL_FALSE,0,reinterpret_cast<void*>(0));

	glGenBuffers(1, &TB);
	glBindBuffer(GL_ARRAY_BUFFER, TB);

	glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	std::cout << "QUAD INITED" << std::endl;
}

void Quad::render() const {    
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Quad::bindVAO() const {
	glBindVertexArray(VAO);
}

void Quad::setTexture(std::string tex) {
	textureName = tex;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId(textureName));

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
