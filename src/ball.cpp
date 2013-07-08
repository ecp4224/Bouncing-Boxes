#include "ball.h"

const float FRICTION = .1;
const int BOX_SIZE = 50;
float oldx, oldy, oldz;

ball::ball() {
	x = 50;
	y = 50;
	z = 50;
	yvel = 1;
	xvel = 0;
	zvel = 0;
	size = 2.0f;
	bouncy = .8;
	complete = false;
}

ball::ball(float x, float y) {
	this->x = x;
	this->y = y;
}


ball::~ball() { }

float ball::getX() {
	return x;
}

float ball::getY() {
	return y;
}

float ball::getZ() {
	return z;
}

void ball::draw() {
	glBegin(GL_QUADS);
        glVertex3f(x-size, y+size, z+size);
        glVertex3f(x+size, y+size, z+size);
        glVertex3f(x+size, y-size, z+size);
        glVertex3f(x-size, y-size, z+size);

		glVertex3f(x-size, y+size, z-size);
        glVertex3f(x+size, y+size, z-size);
        glVertex3f(x+size, y-size, z-size);
        glVertex3f(x-size, y-size, z-size);

		glVertex3f(x+size, y-size, z+size);
        glVertex3f(x+size, y+size, z+size);
        glVertex3f(x+size, y+size, z-size);
        glVertex3f(x+size, y-size, z-size);

		glVertex3f(x-size, y-size, z+size);
        glVertex3f(x-size, y+size, z+size);
        glVertex3f(x-size, y+size, z-size);
        glVertex3f(x-size, y-size, z-size);

		glVertex3f(x-size, y+size, z+size);
        glVertex3f(x+size, y+size, z+size);
        glVertex3f(x+size, y+size, z-size);
        glVertex3f(x-size, y+size, z-size);

		glVertex3f(x-size, y-size, z+size);
        glVertex3f(x+size, y-size, z+size);
        glVertex3f(x+size, y-size, z-size);
        glVertex3f(x-size, y-size, z-size);

    glEnd();
}

void ball::postDraw(float GRAVITYX, float GRAVITYY, float GRAVITYZ, float box_x, float box_y, float box_z) { 
	yvel += GRAVITYY;
	xvel += GRAVITYX;
	zvel += GRAVITYZ;
	if ((y - yvel) - size <= box_y - BOX_SIZE || (x - xvel) + size >= box_x + BOX_SIZE)
		yvel = -yvel * bouncy;
	if ((x - xvel) - size <= box_x - BOX_SIZE || (x - xvel) + size >= box_x + BOX_SIZE)
		xvel = -xvel * bouncy;
	if ((z - zvel) - size <= box_z - BOX_SIZE || (z - zvel) + size >= box_z + BOX_SIZE)
		zvel = -zvel * bouncy;
	if (((yvel >= -1 && yvel <= 0) || (yvel >= 0 && yvel <= 1)) && y <= (box_y - BOX_SIZE) + size)
		temp++;
	else
		temp = 0;
	if (temp >= 100) {
		complete = true;
		yvel = 0;
		y = box_y - BOX_SIZE;
	}
	y -= yvel;
	x -= xvel;
	z -= zvel;

	oldx = box_x;
	oldy = box_y;
	oldz = box_z;

	while (y - size < box_y - BOX_SIZE) {
		float ydelta = (box_y - BOX_SIZE) - (getY() - size);
		yvel = (yvel + ydelta);
		y += yvel;
	}

	while (y + size > box_y + BOX_SIZE) {
		float ydelta = (box_y + BOX_SIZE) - (getY() + size);
		yvel = (yvel - ydelta);
		y -= yvel;
	}

	while (x - size < box_x - BOX_SIZE) {
		float xdelta = (box_x - BOX_SIZE) - (getX() - size);
		xvel = (xvel + xdelta);
		x += xvel;
	}

	while (x + size > box_x + BOX_SIZE) {
		float xdelta = (box_x + BOX_SIZE) - (getX() + size);
		xvel = (xvel - xdelta);
		x -= xvel;
	}

	while (z - size < box_z - BOX_SIZE) {
		float zdelta = (box_z - BOX_SIZE) - (getZ() - size);
		zvel = (zvel + zdelta);
		z += zvel;
	}

	while (z + size > box_z + BOX_SIZE) {
		float zdelta = (box_z + BOX_SIZE) - (getZ() + size);
		zvel = (zvel - zdelta);
		z -= zvel;
	}
}

bool ball::outsideOfBox(float box_x, float box_y, float box_z) {
	return (((y - yvel) - size <= box_y - BOX_SIZE) 
		|| ((y - yvel) + size >= box_y + BOX_SIZE)) && 
		(((x - xvel) - size <= box_x - BOX_SIZE) 
		|| ((x - xvel) + size >= box_x + BOX_SIZE)) && 
		(((z - zvel) - size <= box_z - BOX_SIZE) 
		|| ((z - zvel) + size >= box_z + BOX_SIZE));
}

float ball::getSize() {
	return size;
}

void ball::setBouncy(float bounce) {
	bouncy = bounce;
}

void ball::setX(float x) {
	this->x = x;
}

void ball::setY(float y) {
	this->y = y;
}

void ball::setZ(float z) {
	this->z = z;
}

void ball::collide(ball* with) {
	float xdelta = with->getX() - getX();
	float ydelta = with->getY() - getY();
	float zdelta = with->getZ() - getZ();

	xvel = (xvel + xdelta) * bouncy;
	yvel = (yvel + ydelta) * bouncy;
	zvel = (zvel + zdelta) * bouncy;
	with->xvel = (with->xvel + -xdelta) * with->bouncy;
	with->yvel = (with->yvel + -ydelta) * with->bouncy;
	with->zvel = (with->zvel + -zdelta) * with->bouncy;
	complete = false;
}