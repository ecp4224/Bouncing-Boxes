#include "render.h"
#include <bass.h>

const int WIDTH = 512;
const int HEIGHT = 512;
const int DEPTH = 512;
const int BALL_SIZE = 100;
const int BOX_SIZE = 50;
const float SEN = .5;
int sound_chan;
int wait;

float GRAVITYX = 0.0, GRAVITYY = 0.05, GRAVITYZ = 0.00;
float box_x, box_y, box_z;
bool up, down, k_left, k_right, in, out, m_up, m_down, m_left, m_right, m_in, m_out, r_roll, l_roll, reset, b_reset;

render::render(char* song, bool play) {
	this->play = play;
	running = setupSDL();

	if (play) {
		BASS_Init(-1, 44100, 0, NULL, NULL);
		HSTREAM stream;
		stream = BASS_StreamCreateFile(false, song, 0, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_PRESCAN);
		sound_chan = stream;
		BASS_ChannelPlay(sound_chan, false);
	}


	srand(time(NULL));
	for (int i = 0; i < BALL_SIZE; i++) {
		balls[i].setX(rand()%BOX_SIZE*2);
		balls[i].setY(rand()%BOX_SIZE*2);
		balls[i].setZ(rand()%BOX_SIZE*2);
	}

	rotx = 0;
	roty = 0;
	rotz = 0;
	box_x = 0;
	box_y = 0;
	box_z = 0;
	posx = 0;
	posy = 0;
	posz = 100;
	max = BALL_SIZE;
}

render::~render() {
	SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
}

void render::checkSDLError(int line) {
	const char *error = SDL_GetError();
    if (*error != '\0')
    {
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
}

bool render::setupSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;
 
    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	    /* Create our window centered at 512x512 resolution */
    mainwindow = SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!mainwindow) /* Die if creation failed */
        return false;
	checkSDLError(__LINE__);
 
    /* Create our opengl context and attach it to our window */
    maincontext = SDL_GL_CreateContext(mainwindow);
    checkSDLError(__LINE__);
 
 
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

	return true;
}

bool render::setupOpengl() {
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90, WIDTH/HEIGHT, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

void render::killItWithFire(string reason) {
	SDL_Quit();
}

void render::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-rotx, 1, 0, 0);
	glRotatef(-roty, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);

	//gluLookAt(posx, posy, posz, box_x, box_y, box_z, 0, 1, 0);
	glTranslatef(-posx, -posy, -posz);
	glRotatef(-rotz, 0, 0, 1);
	glBegin(GL_QUADS);
		glColor4f(0.5, 1.0, 1.0, .3);
		glVertex3f(box_x - BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);

		glColor4f(1.0, 1.0, 0.0, .3);
		glVertex3f(box_x - BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);

		
		glColor4f(1.0, 0.0, 0.0, .3);
		glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);

		glColor4f(1.0, 0.0, 1.0, .3);
		glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);

		glColor4f(0.0, 1.0, 0.0, .3);
		glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y + BOX_SIZE, box_z - BOX_SIZE);

		glColor4f(0.0, 1.0, 1.0, .3);
		glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z + BOX_SIZE);
        glVertex3f(box_x + BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);
        glVertex3f(box_x - BOX_SIZE, box_y - BOX_SIZE, box_z - BOX_SIZE);
    glEnd();
	glRotatef(0, 0, 0, 1);

	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < max; i++) {
		balls[i].draw();
	}

}

int render::execute() {
	if (play)
		updateSounds();
	checkBalls();
	draw();
	SDL_GL_SwapWindow(mainwindow);
	if (checkKeys() < 0)
		return -1;
	return 0;
}

int render::checkKeys() {
		while(SDL_PollEvent(&evt)) {
		switch (evt.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (!b_reset) {
				balls[max].setX((box_x - BOX_SIZE) + rand()%BOX_SIZE);
				balls[max].setY((box_y - BOX_SIZE) + rand()%BOX_SIZE);
				balls[max].setZ((box_z - BOX_SIZE) + rand()%BOX_SIZE);
				max++;
				b_reset = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			b_reset = false;
			break;
		case SDL_MOUSEMOTION:
			if (reset) {
				reset = false;
				break;
			} else {
				rotx += -evt.motion.yrel;
				roty += evt.motion.xrel;
				SDL_WarpMouseInWindow(mainwindow, 512/2, 512/2);
				reset = true;
				break;
			}
		case SDL_KEYDOWN:
			switch (evt.key.keysym.sym) {
			case SDLK_UP:
				up = true;
				break;
			case SDLK_DOWN:
				down = true;
				break;
			case SDLK_LEFT:
				k_left = true;
				break;
			case SDLK_RIGHT:
				k_right = true;
				break;
			case SDLK_SPACE:
				m_up = true;
				break;
			case SDLK_LCTRL:
				m_down = true;
				break;
			case SDLK_a:
				m_left = true;
				break;
			case SDLK_d:
				m_right = true;
				break;
			case SDLK_w:
				m_in = true;
				break;
			case SDLK_s:
				m_out = true;
				break;
			case SDLK_q:
				l_roll = true;
				break;
			case SDLK_e:
				r_roll = true;
				break;
			case SDLK_ESCAPE:
				return -1;
			}
			break;
		case SDL_KEYUP:
			switch (evt.key.keysym.sym) {
			case SDLK_UP:
				up = false;
				break;
			case SDLK_DOWN:
				down = false;
				break;
			case SDLK_LEFT:
				k_left = false;
				break;
			case SDLK_RIGHT:
				k_right = false;
				break;
			case SDLK_SPACE:
				m_up = false;
				break;
			case SDLK_LCTRL:
				m_down = false;
				break;
			case SDLK_a:
				m_left = false;
				break;
			case SDLK_d:
				m_right = false;
				break;
			case SDLK_w:
				m_in = false;
				break;
			case SDLK_s:
				m_out = false;
				break;
			case SDLK_q:
				l_roll = false;
				break;
			case SDLK_e:
				r_roll = false;
				break;
			}
			break;
		case SDL_QUIT:
			return -1;
		}
    }

	if (m_up)
		posy += 2;
	if (m_down)
		posy -= 2;
	if (m_left)
		posx -= 2;
	if (m_right)
		posx += 2;
	if (up)
		box_y += 2;
	if (down)
		box_y -= 2;
	if (k_left)
		box_x -= 2;
	if (k_right)
		box_x += 2;
	if (in)
		box_z += 2;
	if (out)
		box_z -= 2;
	if (m_in)
		posz -= 2;
	if (m_out)
		posz += 2;
	if (r_roll) {
		rotz -= 2;
		GRAVITYX = cos(rotz * 3.141592653589793 / 180.0) * 0.15;
		GRAVITYY = sin(rotz * 3.141592653589793 / 180.0) * 0.15;
	}
	if (l_roll) {
		rotz += 2;
		GRAVITYX = cos(rotz * 3.141592653589793 / 180.0) * 0.15;
		GRAVITYY = sin(rotz * 3.141592653589793 / 180.0) * 0.15;
	}
	return 0;
}
void render::loop() { }

float num;
void render::checkBalls() {
	for (int i = 0; i < max; i++) {
		if (play) {
			if (GRAVITYY > 0)
				balls[i].yvel += num;
			if (GRAVITYX > 0)
				balls[i].xvel += num;
			if (GRAVITYZ > 0)
				balls[i].zvel += num;
		}
		balls[i].postDraw(GRAVITYX, GRAVITYY, GRAVITYZ, box_x, box_y, box_z);
		float xmin = balls[i].getX() - balls[i].getSize();
		float ymin = balls[i].getY() - balls[i].getSize();
		float zmin = balls[i].getZ() - balls[i].getSize();
		float xmax = balls[i].getX() + balls[i].getSize();
		float ymax = balls[i].getY() + balls[i].getSize();
		float zmax = balls[i].getZ() + balls[i].getSize();
		for (int z = 0; z < max; z++) {
			if (balls[z].getX() > xmin && balls[z].getX() < xmax 
				&& balls[z].getY() > ymin && balls[z].getY() < ymax 
				&& balls[z].getZ() > zmin && balls[z].getZ() < zmax && i != z) {
				balls[i].collide(&balls[z]);
			}
		}
	}
}

bool render::isRunning() {
	return running;
}

void render::updateSounds() {
	float buffer[1024 * 4];
	BASS_ChannelGetData(sound_chan, buffer, -2147483645);
	
	num = 0.0f;
	for (int i = 0; i <= 15; i++)
		num += buffer[i];
	num *= (13.0f/15.0f);
}