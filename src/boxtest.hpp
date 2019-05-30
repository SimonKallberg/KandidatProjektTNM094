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
	static ModelLoader * test_box;
	static void init() {
		test_box = new ModelLoader("../Objects/box", "background");
	}
	boxtest() {	};

	void draw() {
		glPushMatrix();
		glTranslatef(Box_x, Box_y, Box_z);
		glPushMatrix();
		glScalef(Box_scale, Box_scale, Box_scale);
		test_box->draw();
		glPopMatrix();
		glPopMatrix();
	}

};

ModelLoader* boxtest::test_box = nullptr;

#endif
