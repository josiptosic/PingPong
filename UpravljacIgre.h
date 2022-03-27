#pragma once

#include "SDL2/SDL.h"
#include "Engine.hpp"

	class UpravljacIgre
	{
	public:
		UpravljacIgre(Display d) {
			d.stvoriKontekst();

			SDL_Delay(3000);
			Display* dP=new Display(d.Sirina,d.Visina);
			dP = &d;

			Igrac* iP = new Igrac(d);
			Igrac i(d);
			iP = &i;

			Loptica* lP = new Loptica(d);
			Loptica lC(d);
			lP = &lC;

			Protivnik* pP = new Protivnik(d,lC);
			Protivnik pC(d,lC);
			pP = &pC;

			KeyboardInput* kInput = new KeyboardInput();
			KeyListener* kL = new KeyListener();

			kInput->addListener(kL);
			kL->dodajPojavu(iP);		
			kInput->e;
			kInput->pogon = true;
			while (kInput->pogon==true) {
				kInput->updateInput();
				
				iP->kretanje();
				lP->kretanje();
				pP->kretanje();
				
				
				d.clear(d.tamnozelena);
				d.crtajBojom(d.bijela);
				d.crtajPravokutnik(iP->x, iP->y, iP->w, iP->h, d.bijela);
				d.crtajPravokutnik(lP->x, lP->y, lP->w, lP->h, d.zuta);
				d.crtajPravokutnik(pP->x, pP->y, pP->w, pP->h, d.bijela);

				d.commit();
				SDL_Delay(42);
			}	
		}
	};

	


