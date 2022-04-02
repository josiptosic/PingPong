#pragma once

#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include "Engine.hpp"

class UpravljacIgre
{
public:
	UpravljacIgre(Display zaslon) {
		zaslon.stvoriKontekst();
		konstrukcijaObjekata(&zaslon);
		dodijeliVrijednostStringovima();
	pocetak:
		igrac.postaviPocetneDimenzije(&zaslon);
		loptica.postaviPocetneDimenzije(&zaslon);
		protivnik.postaviPocetneDimenzije(&zaslon, &loptica);
		while(ptrKInput->pogon){
			ptrKInput->updateInput();
			zaslon.clear(zaslon.tamnoplava);
			zaslon.crtajTekst(uvod, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 2 - 50);
			zaslon.commit();
			if(igrac.stanjeTipke == RAZMAKNICA) {
				while (ptrKInput->pogon) {
					ptrKInput->updateInput();
					otkrivanjeSudara(&zaslon, &loptica, &igrac, &protivnik);
					igrac.kretanje(&zaslon);
					loptica.kretanje(&zaslon);
					protivnik.kretanje(&zaslon, &loptica);
					zaslon.clear(zaslon.tamnoplava);
					zaslon.crtajTekst(to_string(igrac.pogodak), zaslon.Sirina / 4 - 17, zaslon.Visina / 8);
					zaslon.crtajTekst(to_string(protivnik.pogodak), zaslon.Sirina * 3 / 4 - 17, zaslon.Visina / 8);
					zaslon.crtajBojom(zaslon.bijela);
					zaslon.crtajPravokutnik(igrac.x, igrac.y, igrac.w, igrac.h, zaslon.bijela);
					zaslon.crtajPravokutnik(protivnik.x, protivnik.y, protivnik.w, protivnik.h, zaslon.bijela);
					zaslon.crtajPravokutnik(loptica.x, loptica.y, loptica.w, loptica.h, zaslon.bijela);
					zaslon.crtajLiniju(zaslon.Sirina / 2, 0, zaslon.Sirina / 2, zaslon.Visina, zaslon.bijela);
					zaslon.crtajLiniju(zaslon.Sirina / 2 - 1, 0, zaslon.Sirina / 2 - 1, zaslon.Visina, zaslon.bijela);
					zaslon.commit();
					SDL_Delay(1000 / 60);
					while(igrac.pogodak == 3 ) {
						zaslon.clear(zaslon.tamnozelena);
						zaslon.crtajTekst(pobjeda, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 2 - 50);
						zaslon.crtajTekst(revans, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 3 * 2 - 50);
						zaslon.commit();
						SDL_Delay(1000 / 60);
						ptrKInput->updateInput();
						if (igrac.stanjeTipke == RAZMAKNICA) goto pocetak;	
					}
					while(protivnik.pogodak == 3 ) {
						zaslon.clear(zaslon.tamnocrvena);
						zaslon.crtajTekst(poraz, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 2 - 50);
						zaslon.crtajTekst(revans, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 3 * 2 - 50);
						zaslon.commit();
						SDL_Delay(1000 / 60);
						ptrKInput->updateInput();
						if (igrac.stanjeTipke == RAZMAKNICA) goto pocetak;
					}
				}
			}
		}
	}

	void konstrukcijaObjekata(Display* ptrZaslon) {
		ptrIgrac = &igrac;
		Protivnik protivnik;
		ptrKInput = &kInput;
		InputListener* ptrKListener;
		ptrKListener = &kListener;
		kListener.opPojava;
		kInput.kLst;
		ptrKInput->addListener(ptrKListener);
		ptrKListener->dodajPojavu(ptrIgrac);
		ptrKInput->pogon = true;
	}
	void otkrivanjeSudara(Display* d, Loptica* l, Igrac* i, Protivnik* p) {
		if ((l->x <= i->w) && ((l->y + l->h) >= i->y) && (i->y + i->h) >= (l->y + l->h)) { l->dx *= -1; }
		else if (l->x <= i->w) { p->uvecajPogodak(); l->postaviPocetneDimenzije(d); }
		if ((l->x+l->h >= p->x) && ((l->y + l->h) >= p->y) && (p->y + p->h) >= (l->y + l->h)) { l->dx *= -1; }
		else if (l->x + l->w > p->x) { i->uvecajPogodak(); l->postaviPocetneDimenzije(d); }
	}
	/*bool sudarDesniOdbija(Display* ptrZaslon, Loptica* ptrLoptica, Igrac* ptrIgrac, Protivnik* ptrProtivnik) {
	
	}*/
	void dodijeliVrijednostStringovima() {
		uvod = "Press SPACE to start.";
		pobjeda = "You win!";
		poraz = "You lose!";
		revans = "Press SPACE to restart.";
	}
	void iscrtavanjeZavrsnogZaslona(Display zaslon, bool uvjetPobjede) {
		if (uvjetPobjede) {
			zaslon.clear(zaslon.tamnozelena);
			zaslon.crtajTekst(pobjeda, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 2 - 50);
		}
		else {
			zaslon.clear(zaslon.tamnocrvena);
			zaslon.crtajTekst(poraz, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 2 - 50);
		}
		zaslon.crtajTekst(revans, zaslon.Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon.Visina / 3 * 2 - 50);
		zaslon.commit();
		SDL_Delay(1000 / 60);
		ptrKInput->updateInput();
		if (igrac.stanjeTipke == RAZMAKNICA) {
			igrac.postaviPocetneDimenzije(&zaslon);
			loptica.postaviPocetneDimenzije(&zaslon);
			protivnik.postaviPocetneDimenzije(&zaslon, &loptica);
		}
		else iscrtavanjeZavrsnogZaslona(zaslon, uvjetPobjede);
	}
	
	Igrac igrac;
	Loptica loptica;
	Protivnik protivnik;
	KeyboardInput kInput;
	KeyListener kListener;

	Display* ptrZaslon;
	Igrac* ptrIgrac;
	Loptica* ptrLoptica;
	Protivnik* ptrProtivnik;
	KeyboardInput* ptrKInput;
	InputListener* ptrKListener;

	string uvod;
	string pobjeda;
	string poraz;
	string revans;

	/*
	bool sudarDesniOdbija = (ptrLoptica->x <= ptrIgrac->w)
							&& ((ptrLoptica->y + ptrLoptica->h) >= ptrIgrac->y)
							&& (ptrIgrac->y + ptrIgrac->h) >= (ptrLoptica->y + ptrLoptica->h);
	bool sudarLijeviOdbija = (ptrLoptica->x + ptrLoptica->h >= ptrProtivnik->x)
							&& ((ptrLoptica->y + ptrLoptica->h) >= ptrProtivnik->y)
							&& (ptrProtivnik->y + ptrProtivnik->h) >= (ptrLoptica->y + ptrLoptica->h);
	bool sudarDesniPogodak = ptrLoptica->x + ptrLoptica->w > ptrProtivnik->x;
	bool sudarLijeviPogodak = ptrLoptica->x <= ptrIgrac->w;
	bool uvjetPobjede = igrac.pogodak == 3;
	bool uvjetPoraza = protivnik.pogodak == 3;
	*/

	
};

	


