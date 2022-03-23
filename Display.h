#include <SDL2/SDL.h>
#include <iostream>

#pragma once


namespace CircuitMessPong {

	class Display
	{
	protected:

		SDL_Window* Prozor;
		Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
		SDL_Renderer* frameBuffer;
		bool pogon = true;


	public:

		Display(int sirina, int visina) {
			Sirina = sirina;
			Visina = visina;

			SDL_Init(SDL_INIT_VIDEO);

			Prozor = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Sirina, Visina, 0);
			if (!Prozor) std::cout << "Dogodila se greška prilikom stvaranja prozora!" << std::endl;
			frameBuffer = SDL_CreateRenderer(Prozor, -1, render_flags);
			if (!frameBuffer) { std::cout << "Dogodila se greška prilikom stvaranja frameBuffera!" << std::endl; SDL_DestroyWindow(Prozor); }
		}

		~Display() {
			SDL_DestroyWindow(Prozor);
			SDL_Quit();
		}


		int Sirina;
		int Visina;



		struct boja {
			int r = 0, g = 0, b = 0;
		};

		boja crna,
			crvena{ crvena.r = 255 },
			zelena{ zelena.g = 255 },
			cijan{ cijan.g = 255, cijan.b = 255 },
			ljubicasta{ ljubicasta.r = 255, ljubicasta.b = 255 },
			zuta{ zuta.r = 255, zuta.g = 255 },
			bijela{ bijela.r = 255, bijela.g = 255, bijela.b = 255 },
			tamnocrvena{ tamnocrvena.r = 86 },
			tamnozelena{ tamnozelena.g = 86 },
			tamnoplava{ tamnoplava.b = 86 };




		void crtajPravokutnik(int polozajX, int polozajY, int sirina, int visina, boja b) {

			SDL_Rect pravokutnik;
			pravokutnik.x = polozajX;
			pravokutnik.y = polozajY;
			pravokutnik.w = sirina;
			pravokutnik.h = visina;
			crtajBojom(b);
			SDL_RenderDrawRect(frameBuffer, &pravokutnik);
		}

		void crtajBojom(boja b) {

			SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
		}


		void clear(boja b) {
			crtajBojom(b);
			SDL_RenderClear(frameBuffer);
		}

		void commit() {
			SDL_RenderPresent(frameBuffer);
		}


	};

}