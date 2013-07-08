#include "cube.h"
#include "render.h"
#include "input.h"

int main( int argc, char* args[] ) {
	cout<<"Setting up SDL.."<<endl;

	render draw("song1.mp3", false);
	input keys;

	cout<<"Setting up OpenGL.."<<endl;
	draw.setupOpengl();

	cout<<"Setting up tickers.."<<endl;
	while (true) {
		if (draw.execute() < 0)
			break;
	}

	return 0;
}