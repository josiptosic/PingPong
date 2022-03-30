#pragma once

#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
//#include <SDL2/begin_code.h>
#include "Engine.hpp"

	class UpravljacIgre
	{
	public:
		UpravljacIgre(Display d) {
			
			
			d.stvoriKontekst();
		pocetak:
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
			KeyboardInput kInput;
			KeyboardInput* pKInput=&kInput;
			KeyListener kL;
			InputListener* pKL;//=&kL;
			pKL = &kL;
			kL.opPojava;
			kInput.kLst;

			pKInput->addListener(pKL);
			pKL->dodajPojavu(iP);		
			//pKInput->e;
			pKInput->pogon = true;
			string uvod = "Press SPACE to start.";
			string pobjeda = "You win!";
			string poraz = "You lose!";
			string revans = "Press SPACE to restart.";

			while (pKInput->pogon==true) {
				pKInput->updateInput();
				d.clear(d.tamnoplava);
				d.crtajTekst(uvod, d.Sirina / 2 - strlen(uvod.c_str()) * 7, d.Visina / 2 - 50);
				d.commit();
				if (i.stanjeTipke == RAZMAKNICA) {
					while (pKInput->pogon == true) {
						pKInput->updateInput();

						otkrivanjeSudara(&d, &lC, &i, &pC);

						i.kretanje(&d);
						lC.kretanje(&d);
						pC.kretanje(&d, &lC);

						d.clear(d.tamnoplava);
						d.crtajTekst(to_string(i.pogodak), d.Sirina / 4 - 17, d.Visina / 8);
						d.crtajTekst(to_string(pC.pogodak), d.Sirina * 3 / 4 - 17, d.Visina / 8);
						d.crtajBojom(d.bijela);
						d.crtajPravokutnik(i.x, i.y, i.w, i.h, d.bijela);
						d.crtajPravokutnik(pC.x, pC.y, pC.w, pC.h, d.bijela);
						d.crtajPravokutnik(lC.x, lC.y, lC.w, lC.h, d.bijela);
						d.crtajLiniju(d.Sirina / 2, 0, d.Sirina / 2, d.Visina, d.bijela);
						d.crtajLiniju(d.Sirina / 2 - 1, 0, d.Sirina / 2 - 1, d.Visina, d.bijela);
						d.commit();
						SDL_Delay(1000 / 60);
						
						while (i.pogodak == 3) {
							d.clear(d.tamnozelena);
							d.crtajTekst(pobjeda, d.Sirina / 2 - strlen(uvod.c_str()) * 7, d.Visina / 2 - 50);
							d.crtajTekst(revans, d.Sirina / 2 - strlen(uvod.c_str()) * 7, d.Visina / 3 * 2 - 50);
							d.commit();
							pKInput->updateInput();
							if (i.stanjeTipke == RAZMAKNICA) goto pocetak;	
						}
						
						while (pC.pogodak == 3) {
							d.clear(d.tamnocrvena);
							d.crtajTekst(poraz, d.Sirina / 2 - strlen(uvod.c_str()) * 7, d.Visina / 2 - 50);
							d.crtajTekst(revans, d.Sirina / 2 - strlen(uvod.c_str()) * 7, d.Visina / 3 * 2 - 50);
							d.commit();
							pKInput->updateInput();
							if (i.stanjeTipke == RAZMAKNICA) goto pocetak;
						}
						
					}
				}
			}
		}

		void otkrivanjeSudara(Display* d, Loptica* l, Igrac* i, Protivnik* p) {
			
			if ((l->x <= i->w) && ((l->y + l->h) >= i->y) && (i->y + i->h) >= (l->y + l->h)) { l->dx *= -1; }
			else if (l->x <= i->w) { p->uvecajPogodak(); l->postaviPocetneDimenzije(d); }
			if ((l->x+l->h >= p->x) && ((l->y + l->h) >= p->y) && (p->y + p->h) >= (l->y + l->h)) { l->dx *= -1; }
			else if (l->x + l->w > p->x) { i->uvecajPogodak(); l->postaviPocetneDimenzije(d); }
			
		}
	};

	


