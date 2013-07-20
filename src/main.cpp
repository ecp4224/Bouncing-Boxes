#include "cube.h"
#include "render.h"
#include "input.h"

int main( int argc, char* args[] ) {
	if(argc !=2){
		cout<<"Usage: ./bouncingboxes path/to/music/file"<<endl;
		return 1;
	}
	
	cout<<"Setting up SDL.."<<endl;

	render draw(args[1], false);
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
