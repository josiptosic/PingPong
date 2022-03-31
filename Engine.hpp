#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Display
{
private:
	SDL_Window* Prozor;
	Uint32 render_flags;
	SDL_Renderer* frameBuffer;
	TTF_Font* Font;

public:
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
		TTF_Init();
		Font = TTF_OpenFont("fonts/Orbitron-Bold.ttf", 24);
		Prozor = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Sirina, Visina, 0);
		if (!Prozor) std::cout << "Dogodila se greška prilikom stvaranja prozora!" << std::endl;
		frameBuffer = SDL_CreateRenderer(Prozor, -1, render_flags);
		if (!frameBuffer) { std::cout << "Dogodila se greška prilikom stvaranja frameBuffera!" << std::endl; SDL_DestroyWindow(Prozor); }
	}
	void crtajLiniju(int xP, int yP, int xZ, int yZ, boja b) {
		SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
		SDL_RenderDrawLine(frameBuffer, xP, yP, xZ, yZ);
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
	void crtajTekst(string poruka, int x, int y){
		SDL_Color White = { 255, 255, 255 };
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, poruka.c_str(), White);
		SDL_Texture* Message = SDL_CreateTextureFromSurface(frameBuffer, surfaceMessage);
		SDL_Rect Message_rect;

		Message_rect.x = x;   
		Message_rect.y = y; 
		Message_rect.w = 15 * strlen(poruka.c_str()); 
		Message_rect.h = 50; 

		SDL_RenderCopy(frameBuffer, Message, NULL, &Message_rect);
		SDL_RenderDrawRect(frameBuffer, &Message_rect);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
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
		pogodak = 0;
	}
	void postaviStanje(Key K) {
		stanjeTipke = K;
	}
	void smjer(Key K) {
		if (K == GORE) {
			dy = -10;
		}
		else if (K == DOLJE) {
			dy = 10;
		}
		else dy = 0;
	}
	void kretanje(Display* d) {
		if (((y > 0) && ((y + h) < d->Visina)) || (y <= 0 && dy > 0) || (y + h >= d->Visina && dy < 0)) y += dy;
		smjer(stanjeTipke);
	}
	void uvecajPogodak() {
		pogodak += 1;
	}

	int pogodak;
};

class InputListener
{
public:
	/*void buttonPressed(Key K) {}
	void buttonReleased(Key K) {}*/
	
	virtual void dodajPojavu(Igrac* p) = 0;
	virtual void buttonPressed(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi pritisak tipke
	virtual void buttonReleased(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi otpuštanje tipke
};

class Input
{
public:
	virtual void addListener(InputListener* listener) = 0;
	virtual void removeListener(InputListener* listener) = 0;

	list<InputListener*> listeners;
	bool pogon;
};


class Loptica : public Pojava {
public:
	int dx = 10;

	void postaviPocetneDimenzije(Display* d) {
		x = d->Sirina / 2, y = d->Visina / 2;
		w = 20; h = 20; dx = 5, dy = -5;

	}
	void smjer(Display d, Key K) {}
	void smjer(Display* d) {
		if (x == d->Sirina) { x = d->Sirina / 2; y = d->Visina / 2; dx *= -1; dy *= -1; }
		else if (x == 0) { x = d->Sirina / 2; y = d->Visina / 2; dx *= 1; dy *= 1; }
		else if (y == d->Visina - h) { dy *= -1; }
		else if (y == 0) { dy *= -1; }
		else if (x >= d->Sirina) { postaviPocetneDimenzije(d); }
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
	void postaviPocetneDimenzije(Display* d, Loptica* l) {
		pogodak = 0;
		w = 20; h = 100;
		x = d->Sirina - 20;
		y = d->Visina / 2 - h / 2;
		dy = 4;
		if (l->dy < 0) { dy *= -1; }
	}
	void smjer(Key K){}
	void smjer(Display d) {}
	void smjer(Display d, Key K) {}
	void smjer(Display* d, Loptica* l) {
		if ((l->dy < 0 && dy > 0) || (l->dy > 0 && dy < 0)) dy *= -1;
		
		if (y > l->y) { dy = -4; }
		else { dy = 4; }
		
		if (d->Visina <= (y + h)) { dy = 0; }
		else if (y <= 0) { dy = 0; }

		if (dy == 0 && y == 0) { dy = 4; }
		
		if (dy == 0 && (y + h) >= d->Visina) { dy = -4; }
		
		if (l->y > y + h) { dy = 4; }
		
		if (l->y > y && ((y + h) >= d->Visina)) { dy = 0; }
	}
	void kretanje(Display* d, Loptica* l) {
		y += dy;
		smjer(d, l);
	}
	void uvecajPogodak() {
		pogodak += 1;
	}

	int pogodak;
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
		promijeniStanje(NISTA);
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
	void updateInput() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				updateListeners(e);
				break;
			case SDL_KEYUP:
				updateListeners(e);
				break;
			case SDL_QUIT: pogon = false;
			default:
				break;
			}
		}
	}
	void addListener(InputListener* listener) { kLst.push_back((KeyListener*)listener); }
	void removeListener(InputListener* listener) { kLst.remove((KeyListener*)listener); }
	void updateListeners(SDL_Event e) {
		for (KeyListener* l : kLst) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_SPACE: l->buttonPressed(RAZMAKNICA); break;
				case SDL_SCANCODE_UP: l->buttonPressed(GORE); break;
				case SDL_SCANCODE_DOWN: l->buttonPressed(DOLJE); break;
				default: break;
				}
			}
			else if (e.type == SDL_KEYUP) {
				l->buttonReleased(NISTA);
			}
		}
	}
	list<KeyListener*> kLst;
};








