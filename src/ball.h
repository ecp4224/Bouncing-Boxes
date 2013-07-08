#include "da_magicz"

class ball
{
public:
	ball(float x, float y);
	ball();
	~ball();
	void postDraw(float GRAVITYX, float GRAVITYY, float GRAVITYZ, float box_x, float box_y, float box_z);
	void draw();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setBouncy(float bounce);
	float getX();
	float getY();
	float getZ();
	float getSize();
	void collide(ball* with);
	bool outsideOfBox(float box_x, float box_y, float box_z);
	float yvel, xvel, zvel;
private:
	float x, y, z, bouncy, size;
	int temp;
	bool complete;
};

