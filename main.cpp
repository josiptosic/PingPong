#include <SDL2/SDL.h>
#include "Engine.hpp"
#include "UpravljacIgre.h"







int main(int argc, char* argv[]) {
	

	Display zaslon(640, 480);
	UpravljacIgre Pokretanje(zaslon);
	


	/*SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window* window = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;

	SDL_Event e;

	SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);

	while (running) {
		
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
				if (e.key.keysym.scancode == SDL_SCANCODE_W) SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
				break;
			case SDL_MOUSEMOTION:
				SDL_SetRenderDrawColor(renderer, e.motion.x, e.motion.y, 100, 255);
			default:
				break;
			}
		}

		SDL_RenderClear(renderer);

		
		//Ovdje iscrtaj više stvari
		SDL_RenderPresent(renderer);
	}

	SDL_Quit(); 
	*/

	return 0;
}