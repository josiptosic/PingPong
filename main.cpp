#include <SDL2/SDL.h>
#include "Engine.hpp"
#include "UpravljacIgre.h"

int main(int argc, char* argv[]) {
	Display zaslon(640, 480);
	
	UpravljacIgre Pokretanje(zaslon);
	return 0;
}