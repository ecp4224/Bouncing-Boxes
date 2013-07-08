#include "da_magicz"
#include "ball.h"

class render {
public:
	render(char* song, bool play);
	~render();
	bool setupSDL();
	bool isRunning();
	void killItWithFire(string reason);
	bool setupOpengl();
	int execute();
	void loop();
	void addBall(ball b);
	void normal();
private:
	bool play;
	SDL_Window *mainwindow;
	SDL_GLContext maincontext;
	SDL_Event evt;
	ball balls[2000];
	int max;
	bool running;
	void checkSDLError(int line);
	void draw();
	void updateSounds();
	int checkKeys();
	float rotx, roty, rotz, posx, posy, posz;
	void checkBalls();
};