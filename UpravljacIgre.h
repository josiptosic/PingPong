#pragma once

#include "SDL2/SDL.h"
#include "Engine.hpp"

	class UpravljacIgre
	{
	public:
		UpravljacIgre(Display d) {
			d.stvoriKontekst();
			
			Display* dP;;
			dP = &d;
			
			Igrac i;
			Igrac* iP = &i;// = new Igrac();
			
			i.postaviPocetneDimenzije(&d);
			//SDL_Delay(3000);
			//iP->postaviPocetneDimenzije(d);// iP = i;

			//Loptica* lP = new Loptica();
			Loptica lC;
			lC.postaviPocetneDimenzije(&d);
			//lP = &lC;
			
			//Protivnik* pP = new Protivnik();
			Protivnik pC;
			pC.postaviPocetneDimenzije(&d, &lC);
			//pP = &pC;

			KeyboardInput* kInput = new KeyboardInput();
			KeyListener* kL = new KeyListener();

			kInput->addListener(kL);
			kL->dodajPojavu(iP);		
			kInput->e;
			kInput->pogon = true;

			while (kInput->pogon==true) {
				kInput->updateInput();
				
				otkrivanjeSudara(&lC, &i, &pC);
				i.kretanje();
				lC.kretanje(&d);
				pC.kretanje(&d, &lC);
				
				d.clear(d.tamnoplava);
				d.crtajBojom(d.bijela);
				d.crtajPravokutnik(i.x, i.y, i.w, i.h, d.bijela);
				d.crtajPravokutnik(pC.x, pC.y, pC.w, pC.h, d.bijela);
				d.crtajPravokutnik(lC.x, lC.y, lC.w, lC.h, d.bijela);
				d.commit();
				SDL_Delay(50);
			}	
		}

		void otkrivanjeSudara(Loptica* l, Igrac* i, Protivnik* p) {
			if ((l->x <= i->w) && ((l->y + l->h) >= i->y) && (i->y + i->h) >= (l->y + l->h)) { l->dx *= -1; }
			if ((l->x >= p->x) && ((l->y + l->h) >= p->y) && (p->y + p->h) >= (l->y + l->h)) { l->dx *= -1; }
		}
	};

	


