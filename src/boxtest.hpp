#ifndef BOXTEST
#define BOXTEST

#include <iostream>
#include "./ModelLoader.hpp"

class boxtest {
public:
	float Box_x = 0.0f;
	float Box_y = 0.0f;
	float Box_z = -5.0f;
	float Box_scale = 1.0f;
	ModelLoader *test_box;
	boxtest(std::string in_rootDir): rootDir(in_rootDir){
		test_box = new ModelLoader(rootDir + "/Objects/box", "background");
	}

	void draw() {
		glPushMatrix();
		glTranslatef(Box_x, Box_y, Box_z);
		glPushMatrix();
		glScalef(Box_scale, Box_scale, Box_scale);
		test_box->draw();
		glPopMatrix();
		glPopMatrix();
	}
private:
    std::string rootDir = "";
};

#endif
