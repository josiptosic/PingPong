#include "Engine.hpp"
#include <SDL2/SDL.h>
#include "UpravljacIgre.h"

int main(int argc, char* argv[]) {
	Display zaslon(640, 480);
	UpravljacIgre Pokretanje(zaslon);
	zaslon.~Display();
	return 0;
}