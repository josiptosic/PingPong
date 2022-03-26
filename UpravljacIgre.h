#pragma once

#include "SDL2/SDL.h"
#include "Engine.hpp"

	class UpravljacIgre
	{
	public:
		UpravljacIgre(Display d) {
			//SDL_Init(SDL_INIT_VIDEO);
			
			d.stvoriKontekst();

			
			Igrac* i1 = new Igrac();
			Loptica* l = new Loptica();
			Protivnik* i2 = new Protivnik();
			KeyboardInput* kInput = new KeyboardInput();
			KeyListener* kL = new KeyListener();
			kInput->addListener(kL);
			kL->dodajPojavu(i1);		
			kInput->e;
			kInput->pogon = true;
			while (kInput->pogon==true) {
				kInput->updateInput();
				
				i1->kretanje();
				l->kretanje();
				i2->kretanje();
				
				
				d.clear(d.tamnozelena);
				d.crtajBojom(d.bijela);
				d.crtajPravokutnik(i1->x, i1->y, i1->w, i1->h, d.bijela);
				d.crtajPravokutnik(l->x, l->y, l->w, l->h, d.zuta);
				d.crtajPravokutnik(i2->x, i2->y, i2->w, i2->h, d.bijela);
				d.commit();
				SDL_Delay(42);
			}	
		}
	};

	


