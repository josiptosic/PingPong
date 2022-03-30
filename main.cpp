#include "Engine.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/begin_code.h>
#include "UpravljacIgre.h"

int main(int argc, char* argv[]) {
	
/*	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Window* Prozor;
	SDL_Renderer* renderer;

	Prozor = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	//if (!Prozor) std::cout << "Dogodila se greška prilikom stvaranja prozora!" << std::endl;
	renderer = SDL_CreateRenderer(Prozor, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	
	TTF_Font* Sans = TTF_OpenFont("fonts/Orbitron-Bold.ttf", 24);

	// this is the color in rgb format,
	// maxing out all would give you the color white,
	// and it will be your text's color
	SDL_Color White = { 255, 255, 255 };

	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(Sans, "1", White);

	// now you can convert it into a texture
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect Message_rect; //create a rect
	
		Message_rect.x = 0;  //controls the rect's x coordinate 
		Message_rect.y = 0; // controls the rect's y coordinte
		Message_rect.w = 35; // controls the width of the rect
		Message_rect.h = 100; // controls the height of the rect
		

		// Don't forget to free your surface and texture
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
		SDL_RenderDrawRect(renderer, &Message_rect);
		
		SDL_RenderPresent(renderer);
		SDL_Delay(3000);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	return 0;
	*/
	Display zaslon(640, 480);
	UpravljacIgre Pokretanje(zaslon);
	zaslon.~Display();
	return 0;
	

}