#pragma once

#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include "Engine.hpp"
#include <time.h>

class UpravljacIgre
{

public:
	UpravljacIgre(Display zaslon) {
		zaslon.stvoriKontekst();
		konstrukcijaObjekata(&zaslon);
		dodijeliVrijednostStringovima();
		igrac.postaviPocetneDimenzije(&zaslon);
		loptica.postaviPocetneDimenzije(&zaslon);
		protivnik.postaviPocetneDimenzije(&zaslon, &loptica);
		pocetniZaslon(&zaslon, &igrac, &loptica, &protivnik, stanjePogona(ptrKInput));
	}

	void konstrukcijaObjekata(Display* ptrZaslon) {								//Konstrukcija objekata pojava i Listener èlanova
		ptrIgrac = &igrac;
		ptrKInput = &kInput;
		ptrKInput->pogon = true;
		ptrKListener = &kListener;
		ptrKInput->addListener(ptrKListener);
		ptrKListener->dodajPojavu(ptrIgrac);
	}

	bool stanjePogona(KeyboardInput* ptrKInput) {								//Funkcija uvjeta istinitosti rada igre
		return ptrKInput->pogon;
	}

	bool uvjetPobjede(Igrac* ptrIgrac) {										//Funkcija uvjeta istinitosti pobjede igraèa
		return ptrIgrac->pogodak == 3;
	}

	bool uvjetPoraza(Protivnik* ptrProtivnik) {									//Funkcija uvjeta istinitosti pobjede raèunala
		return ptrProtivnik->pogodak == 3;
	}

	bool odbijanjeLopticeOdIgraca(Igrac* ptrIgrac, Loptica* ptrLoptica) {		//Funkcija uvjeta istinitosti odbijanja pojave 'Loptica' od pojave 'Igrac'
		return (ptrLoptica->x <= ptrIgrac->w)
			&& ((ptrLoptica->y + ptrLoptica->h) >= ptrIgrac->y)
			&& (ptrIgrac->y + ptrIgrac->h) >= (ptrLoptica->y);
	}

	bool odbijanjeLopticeOdProtivnika(Loptica* ptrLoptica, Protivnik* ptrProtivnik) {	//Funkcija uvjeta istinitosti odbijanja pojave 'Loptica' od pojave 'Protivnik'
		return (ptrLoptica->x + ptrLoptica->w >= ptrProtivnik->x)
			&& ((ptrLoptica->y + ptrLoptica->h) >= ptrProtivnik->y)
			&& (ptrProtivnik->y + ptrProtivnik->h) >= (ptrLoptica->y);
	}

	bool pogodakIgraca(Loptica* ptrLoptica, Protivnik* ptrProtivnik) {			//Funkcija uvjeta istinitosti postignutog pogotka igraèa
		return (ptrLoptica->x + ptrLoptica->w > ptrProtivnik->x);
	}

	bool pogodakProtivnika(Igrac* ptrIgrac, Loptica* ptrLoptica) {				//Funkcija uvjeta istinitosti postignutog pogotka raèunala
		return (ptrLoptica->x <= ptrIgrac->w);
	}

	void pocetniZaslon(Display* zaslon, Igrac* ptrIgrac, Loptica* ptrLoptica, Protivnik* ptrProtivnik, bool stanjePogona) {	//Funkcija petlje iscrtavanja poèetnog zaslona
		while (ptrIgrac->stanjeTipke != RAZMAKNICA) {
			zaslon->clear(zaslon->tamnoplava);
			zaslon->crtajTekst(uvod, zaslon->Sirina / 2 - strlen(uvod.c_str()) * 7, zaslon->Visina / 2 - 50);
			zaslon->commit();
			zaslon->odgoda(60);
			ptrKInput->updateInput();
		}
		srand(time(0));
		igra(zaslon, ptrIgrac, ptrLoptica, ptrProtivnik);
	}

	void igra(Display* zaslon, Igrac* ptrIgrac, Loptica* ptrLoptica, Protivnik* ptrProtivnik) {	//Funkcija petlje iscrtavanja igre u izvoðenju postignut proceduralnom generacijom
		
		while (!(uvjetPobjede(ptrIgrac) || uvjetPoraza(ptrProtivnik))) {
			otkrivanjeSudara(zaslon, ptrLoptica, ptrIgrac, ptrProtivnik);
			ptrKInput->updateInput();
			ptrIgrac->kretanje(zaslon);
			ptrLoptica->kretanje(zaslon);
			ptrProtivnik->kretanje(zaslon, &loptica);
			zaslon->clear(zaslon->tamnoplava);
			zaslon->crtajTekst(to_string(ptrIgrac->pogodak), zaslon->Sirina / 4 - 17, zaslon->Visina / 8);
			zaslon->crtajTekst(to_string(ptrProtivnik->pogodak), zaslon->Sirina * 3 / 4 - 17, zaslon->Visina / 8);
			zaslon->crtajBojom(zaslon->bijela);
			zaslon->crtajPravokutnik(ptrIgrac->x, ptrIgrac->y, ptrIgrac->w, ptrIgrac->h, zaslon->bijela);
			zaslon->crtajPravokutnik(ptrProtivnik->x, ptrProtivnik->y, ptrProtivnik->w, ptrProtivnik->h, zaslon->bijela);
			zaslon->crtajPravokutnik(ptrLoptica->x, ptrLoptica->y, ptrLoptica->w, ptrLoptica->h, zaslon->bijela);
			zaslon->crtajLiniju(zaslon->Sirina / 2, 0, zaslon->Sirina / 2, zaslon->Visina, zaslon->bijela);
			zaslon->crtajLiniju(zaslon->Sirina / 2 - 1, 0, zaslon->Sirina / 2 - 1, zaslon->Visina, zaslon->bijela);
			zaslon->commit();
			zaslon->odgoda(60);
		}
		iscrtavanjeZavrsnogZaslona(zaslon, &igrac, &loptica, &protivnik, uvjetPobjede(ptrIgrac), uvjetPoraza(ptrProtivnik));
	}



	void otkrivanjeSudara(Display* ptrZaslon, Loptica* ptrLoptica, Igrac* ptrIgrac, Protivnik* ptrProtivnik) {	//Funkcija otkrivanje sudara loptice i ostalih pojava
		if (odbijanjeLopticeOdIgraca(ptrIgrac, ptrLoptica)) { ptrLoptica->dx *= -1; ptrLoptica->promjenaDinamikeKretanja(6);}
		else if (pogodakProtivnika(ptrIgrac,ptrLoptica)) {
			ptrProtivnik->uvecajPogodak();
			ptrLoptica->postaviPocetneDimenzije(ptrZaslon);
			ptrLoptica->y = rand() % (ptrZaslon->Visina - ptrLoptica->h);
		}

		if (odbijanjeLopticeOdProtivnika(ptrLoptica, ptrProtivnik)) { ptrLoptica->dx *= -1; ptrLoptica->promjenaDinamikeKretanja(6); }
		else if (pogodakIgraca(ptrLoptica, ptrProtivnik)) { 
			ptrIgrac->uvecajPogodak(); 
			ptrLoptica->postaviPocetneDimenzije(ptrZaslon);
			ptrLoptica->y = rand() % (ptrZaslon->Visina - ptrLoptica->h);
		}
	}
	
	void dodijeliVrijednostStringovima() {		//Vrijednosti stringova tekstualnih natpisa
		uvod = "Press SPACE to start.";
		pobjeda = "You win!";
		poraz = "You lose!";
		revans = "Press SPACE to restart.";
	}

	void iscrtavanjeZavrsnogZaslona(Display* ptrZaslon, Igrac* ptrIgrac, Loptica* ptrLoptica, Protivnik* ptrProtivnik, bool uvjetPobjede, bool uvjetPoraza) {
		//Funkcija petlje iscrtavanja završnog zaslona
		while (ptrIgrac->stanjeTipke != RAZMAKNICA) {
			if (uvjetPobjede) {
				ptrZaslon->clear(ptrZaslon->tamnozelena);
				ptrZaslon->crtajTekst(pobjeda, ptrZaslon->Sirina / 2 - strlen(uvod.c_str()) * 7, ptrZaslon->Visina / 2 - 50);
			}
			else if (uvjetPoraza) {
				ptrZaslon->clear(ptrZaslon->tamnocrvena);
				ptrZaslon->crtajTekst(poraz, ptrZaslon->Sirina / 2 - strlen(uvod.c_str()) * 7, ptrZaslon->Visina / 2 - 50);
			}
			ptrZaslon->crtajTekst(revans, ptrZaslon->Sirina / 2 - strlen(uvod.c_str()) * 7, ptrZaslon->Visina / 3 * 2 - 50);
			ptrZaslon->commit();
			ptrZaslon->odgoda(60);
			ptrKInput->updateInput();
		}
		
		ptrIgrac->postaviPocetneDimenzije(ptrZaslon);
		ptrLoptica->postaviPocetneDimenzije(ptrZaslon);
		ptrProtivnik->postaviPocetneDimenzije(ptrZaslon, ptrLoptica);
		pocetniZaslon(ptrZaslon, ptrIgrac, ptrLoptica, ptrProtivnik, ptrKInput->pogon);
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

};

	


