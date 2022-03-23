#include "SDL2/SDL.h"
#include "Display.h"
#include "Input.h"

#pragma once

namespace CircuitMessPong {

	class Pojava {


	public:
		
		virtual void smjer(Display d) = 0;

		SDL_Rect p;

		int x, y;
		int w, h;
		int dy;
		

	};

	class Igrac : public Pojava {
	public:
		Igrac(Display d) {
			pogodak = 0;
			x = 0;
			y = d.Visina / 2;
			w = 20; h = 100;
		}

		int pogodak;

		void smjer(Display d, Key K){
			if (K == DOLJE) {
				dy = 20;
			}
			else if (K == GORE) {
				dy = -20;
			}
			else dy = 0;
		}

		void smjer(Display d) {}

	};

	class Protivnik : public Pojava {
	public:
		
		Protivnik(Display d, Loptica l) {
			pogodak = 0;
			x = d.Sirina - 20;
			y = d.Visina / 2;
			w = 20; h = 100;
			dy = 20;
			if (l.dy < 0) { dy *= -1; }
		}

		int pogodak;

		void smjer(Display d) {}

	};

	class Loptica : public Pojava {
	public:
		int dx = 10; 


		Loptica(Display d) {
			x = d.Sirina, y = d.Visina;
			w = 20; h = 20;
			smjer(d);
		}
		
		
		void smjer(Display d) {
			if (x == d.Sirina) { x = d.Sirina / 2; y = d.Visina / 2; dx *= -1; dy *= -1; }
			else if (x == 0) { x = d.Sirina / 2; y = d.Visina / 2; dx *= 1; dy *= 1; }
			else { x += dx; y += dy; }
		}
		
	};

}