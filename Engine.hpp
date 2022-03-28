#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <list>

using namespace std;

class Display
{
public:
	SDL_Window* Prozor;
	Uint32 render_flags;
	SDL_Renderer* frameBuffer;

	Display(int sirina, int visina) {
		Sirina = sirina;
		Visina = visina;
		render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
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
		crvena{ crvena.r = 255, crvena.g = 0, crvena.b = 0 },
		zelena{ zelena.r = 0, zelena.g = 255, zelena.b = 0 },
		plava{ plava.r = 0, plava.g = 0, plava.b = 255 },
		cijan{ cijan.r = 0, cijan.g = 255, cijan.b = 255 },
		ljubicasta{ ljubicasta.r = 255, ljubicasta.g = 0,ljubicasta.b = 255 },
		zuta{ zuta.r = 255, zuta.g = 255, zuta.b = 0 },
		bijela{ bijela.r = 255, bijela.g = 255, bijela.b = 255 },
		tamnocrvena{ tamnocrvena.r = 86, tamnocrvena.g = 0, tamnocrvena.b = 0 },
		tamnozelena{ tamnozelena.r = 0, tamnozelena.g = 86, tamnozelena.b = 0 },
		tamnoplava{ tamnoplava.r = 0, tamnoplava.g = 0, tamnoplava.b = 86 };

	void stvoriKontekst() {
		SDL_Init(SDL_INIT_VIDEO);
		Prozor = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Sirina, Visina, 0);
		//if (!Prozor) std::cout << "Dogodila se greška prilikom stvaranja prozora!" << std::endl;
		frameBuffer = SDL_CreateRenderer(Prozor, -1, render_flags);
		//if (!frameBuffer) { std::cout << "Dogodila se greška prilikom stvaranja frameBuffera!" << std::endl; SDL_DestroyWindow(Prozor); }
	}
	void crtajPravokutnik(int polozajX, int polozajY, int sirina, int visina, boja b) {
		SDL_Rect pravokutnik;
		pravokutnik.x = polozajX;
		pravokutnik.y = polozajY;
		pravokutnik.w = sirina;
		pravokutnik.h = visina;
		crtajBojom(b);
		
		SDL_RenderDrawRect(frameBuffer, &pravokutnik);
		SDL_RenderFillRect(frameBuffer, &pravokutnik);
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

enum Key {
	NISTA = 0,
	RAZMAKNICA = 41,
	GORE = 82,
	DOLJE = 81
};

class InputListener
{
public:
	InputListener() {

	}

	/*void buttonPressed(Key K) {}
	void buttonReleased(Key K) {}*/

	virtual void buttonPressed(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi pritisak tipke
	virtual void buttonReleased(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi otpuštanje tipke
};

class Input
{
public:

	virtual void addListener(InputListener* listener) = 0;
	virtual void removeListener(InputListener* listener) = 0;

	SDL_Event e;
	list<InputListener*> listeners;
	bool pogon;
};

class Pojava {
public:
	virtual void smjer(Key K) = 0;
	virtual void kretanje(){}
	void postaviStanje(Key K) {
		stanjeTipke = K;
	}

	SDL_Rect p;
	Key stanjeTipke;

	int x, y;
	int w, h;
	int dy;
};

class Igrac : public Pojava {
public:
	
	void postaviPocetneDimenzije(Display* d) {
		w = 20; h = 100;
		x = 0; y = d->Visina / 2 - h / 2;
		smjer(NISTA);
	}

	void smjer(Key K) {
		if (K == DOLJE) {
			dy = 20;
		}
		else if (K == GORE) {
			dy = -20;
		}
		else dy = 0;
	}
	void kretanje() {
		smjer(stanjeTipke);
		y += dy;
	}

	int pogodak;
};


class Loptica : public Pojava {
public:
	int dx = 10;

	void postaviPocetneDimenzije(Display* d) {
		x = d->Sirina / 2, y = d->Visina / 2;
		w = 20; h = 20; dx = 10, dy = -10;

	}
	void smjer(Display d, Key K) {}
	void smjer(Display* d) {
		if (x == d->Sirina) { x = d->Sirina / 2; y = d->Visina / 2; dx *= -1; dy *= -1; }
		else if (x == 0) { x = d->Sirina / 2; y = d->Visina / 2; dx *= 1; dy *= 1; }
		else if (y == d->Visina - h) { dy *= -1; }
		else if (y == 0) { dy *= -1; }
		else if (x >= d->Sirina) { postaviPocetneDimenzije(d); }
		//else { x += dx; y += dy; }
	}
	void smjer(Key K) {}
	void kretanje(Display* d) {

		x += dx;
		y += dy;
		smjer(d);
	}
};

class Protivnik : public Pojava {
public:
	
	
	int pogodak;
	

	void postaviPocetneDimenzije(Display* d, Loptica* l) {
		pogodak = 0;
		w = 20; h = 100;
		x = d->Sirina - 20;
		y = d->Visina / 2 - h / 2;
		dy = 5;
		if (l->dy < 0) { dy *= -1; }
	}

	void smjer(Key K){}
	void smjer(Display d) {}
	void smjer(Display d, Key K) {}
	void smjer(Display* d, Loptica* l) {
		/*
		if ((l->dy < 0 && dy < 0) || (l->dy > 0 && dy < 0)) {
			dy *= -1;
		}*/
		if (y > l->y) { dy = -10; }
		else { dy = 10; }

		if (d->Visina <= (y + h)) { dy = 0; }
		else if (y <= 0) { dy = 0; }
		else if (dy == 0 && (y + h) == d->Visina) { dy = -10; }
		else if (dy == 0 && y == 0) { dy = 10; }
	}
	void kretanje(Display* d, Loptica* l) {
		y += dy;
		smjer(d, l);
	}
};



class KeyListener : public InputListener {
public:
	
	void dodajPojavu(Igrac* p) {
		opPojava = p;
	}
	void buttonPressed(Key K) {
		promijeniStanje(K);
	}
	void buttonReleased(Key K) {
		promijeniStanje(K);
	}
	void dodijeliPojavu(Igrac* p) {
		opPojava = p;
	}
	void promijeniStanje(Key K) {
		opPojava->postaviStanje(K);
	}

	Igrac* opPojava;
};

class KeyboardInput : public Input {
public:
	KeyboardInput() {
		e;
		pogon = true;
	}
	void updateInput() {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				
				updateListeners();
				break;
			case SDL_KEYUP:
				updateListeners();
				break;
			case SDL_QUIT: pogon = false;
			default:
				break;
			}
		}
	}
	void addListener(InputListener* listener) { kLst.push_back((KeyListener*)listener); }
	void removeListener(InputListener* listener) { kLst.remove((KeyListener*)listener); }
	void updateListeners() {
		for (InputListener* l : listeners) {
			switch (e.key.keysym.scancode) {
			case RAZMAKNICA: l->buttonPressed(RAZMAKNICA);
			case GORE: l->buttonPressed(GORE);
			case DOLJE: l->buttonPressed(DOLJE);
			default: break;
			}
		}
	}
	list<KeyListener*> kLst;
};








