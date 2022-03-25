#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <list>

using namespace std;

class Display
{
protected:

	SDL_Window* Prozor;
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
	SDL_Renderer* frameBuffer;



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

enum Key {

	NISTA = 0,
	RAZMAKNICA = 41,
	GORE = 82,
	DOLJE = 81

};



class Pojava {


public:

	virtual void smjer(Key K) = 0;

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
	Igrac(Display d) {
		stanjeTipke = NISTA;
		pogodak = 0;
		x = 0; dy = 10;
		y = d.Visina / 2;
		w = 20; h = 100;
		smjer(stanjeTipke);
	}

	Igrac() {}

	int pogodak;


	


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



};

class Loptica : public Pojava {
public:
	int dx = 10;


	Loptica(Display d) {
		x = d.Sirina, y = d.Visina;
		w = 20; h = 20;
		smjer(d);
	}

	void smjer(Display d, Key K) {}
	void smjer(Display d) {
		if (x == d.Sirina) { x = d.Sirina / 2; y = d.Visina / 2; dx *= -1; dy *= -1; }
		else if (x == 0) { x = d.Sirina / 2; y = d.Visina / 2; dx *= 1; dy *= 1; }
		else { x += dx; y += dy; }
	}
	void smjer(Key K) {}


	void kretanje() {

		x += dx;
		y += dy;

	}

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
	Loptica* l;

	void smjer(Display d) {}

	void smjer(Display d, Key K) {}
	void smjer(Key K) {
		if ((l->dy < 0 && dy < 0) || (l->dy > 0 && dy < 0)) {
			dy *= -1;
		}
	}

	void kretanje() {
		y += dy;
	}

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






class KeyListener : public InputListener {

public:
	
	KeyListener() {
		opPojava = new Igrac();
	}
	
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
	
	//int pritisnutaTipka;

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
		for (InputListener* l  : listeners) {
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







