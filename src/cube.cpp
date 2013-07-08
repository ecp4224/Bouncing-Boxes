#include "cube.h"


cube::cube() { 
	x = 50;
	y = 0;
}


cube::~cube() { }

float cube::getX() {
	return x;
}

float cube::getY() {
	return y;
}

int cube::execute() {
	glTranslatef(x, y, 0.0f);
	glBegin(GL_QUADS);
        glColor3f(1, 0, 0); glVertex3f(-10, 10, 0);
        glColor3f(1, 1, 0); glVertex3f(10, 10, 0);
        glColor3f(1, 0, 1); glVertex3f(10, -10, 0);
        glColor3f(1, 1, 1); glVertex3f(-10, -10, 0);
    glEnd();
	y += 2;
	return 0;
}
