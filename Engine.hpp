#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <list>
#include <cmath>

using namespace std;

class Display							//Razred Display koji sadrži funkcijska i podatkovna svojstva zaslona za iscrtavanje
{
private:
	SDL_Window* Prozor;					//Deklaracija pokazivaèa na strokturu koja sadrži svojstva stvorenog prozora
	Uint32 render_flags;				//Flagovi uputa iscrtavanja
	SDL_Renderer* frameBuffer;			//Deklaracija pokazivaèa na strukturu koja sadrži svojstva spremnika za iscrtavanje
	TTF_Font* Font;						//Deklaracija pokazivaèa a strukturu koja sadrži svojstva fonta za iscrtani tekst
	SDL_Color Bijela;					//Deklaracija strukture koja sadži svojstva boje za iscrtavanje teksta
	SDL_Surface* surfaceMessage;		//Deklaracija pokazivaèa na strukturu koja sadži svojstva površine za iscrtavanje
	SDL_Texture* Message;				//Deklaracija pokazivaèa na strukturu koja sadži svojstva poruke koja se iscrtava kao tekst
	SDL_Rect Message_rect;				//Deklaracija pravokutnika koji sadrži tekst poruke za iscrtavanje
	SDL_Rect pravokutnik;				//Deklaracija strukture koja sadrži svojstva za iscrtavanje pravokutnika

public:
	Display(int sirina, int visina) {
		Sirina = sirina;
		Visina = visina;
		render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	}

	~Display() {
		SDL_DestroyWindow(Prozor);		//Dekonstrukcija prozora
		SDL_Quit();						//Dekonstrukcija elemenata biblioteke za iscrtavanje
		TTF_CloseFont(Font);
	}

	int Sirina;							//Širina zaslona za iscrtavanje izražena u pikselima
	int Visina;							//Visina zaslona za iscrtavanje izražena u pikselima

	struct boja {						//Struktura koja sadrži RGB parametre boje za iscrtavanje
		int r = 0, g = 0, b = 0;		//Parametri udjela osnovnih boja za boju iscrtavanja
	};

	boja crna,																		//Objekti strukture boja za iscrtavanja
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

	void stvoriKontekst() {				//Inicijalizacija komponenti biblioteke za iscrtavanje
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		Font = TTF_OpenFont("fonts/Orbitron-Bold.ttf", 24);
		Prozor = SDL_CreateWindow("PingPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Sirina, Visina, 0);
		if (!Prozor) std::cout << "Dogodila se greška prilikom stvaranja prozora!" << std::endl;
		frameBuffer = SDL_CreateRenderer(Prozor, -1, render_flags);
		if (!frameBuffer) { std::cout << "Dogodila se greška prilikom stvaranja frameBuffera!" << std::endl; SDL_DestroyWindow(Prozor); }
	}
	void crtajLiniju(int xP, int yP, int xZ, int yZ, boja b) {						//Funkcija iscrtavanja linije
		SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
		SDL_RenderDrawLine(frameBuffer, xP, yP, xZ, yZ);
	}
	void crtajPravokutnik(int polozajX, int polozajY, int sirina, int visina, boja b) {	//Funkcija iscrtavanja pravokutnika
		
		pravokutnik.x = polozajX;		//Koordinata poèetne toèke iscrtavanja na X osi
		pravokutnik.y = polozajY;		//Koordinata poèetne toèke iscrtavanja na Y osi
		pravokutnik.w = sirina;			//Širina pravokutnika za iscrtavanje, izražena u pikselima
		pravokutnik.h = visina;			//Visina pravokutnika za iscrtavanje, izražena u pikselima
		crtajBojom(b);
		
		SDL_RenderDrawRect(frameBuffer, &pravokutnik);
		SDL_RenderFillRect(frameBuffer, &pravokutnik);
	}
	void crtajTekst(string poruka, int x, int y){		//Funkcija iscrtavanja teksta na zaslon
		Bijela = { 255, 255, 255 };
		surfaceMessage = TTF_RenderText_Solid(Font, poruka.c_str(), Bijela);
		Message = SDL_CreateTextureFromSurface(frameBuffer, surfaceMessage);

		Message_rect.x = x;				//Koordinata poèetne toèke iscrtavanja na X osi
		Message_rect.y = y;				//Koordinata poèetne toèke iscrtavanja na Y osi
		Message_rect.w = 15 * strlen(poruka.c_str());		//Širina pravokutnika za iscrtavanje teksta, izražena u pikselima
		Message_rect.h = 50;			//Visina pravokutnika za iscrtavanje teksta, izražena u pikselima

		SDL_RenderCopy(frameBuffer, Message, NULL, &Message_rect);
		SDL_RenderDrawRect(frameBuffer, &Message_rect);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
	}
	void crtajBojom(boja b) {			//Boja iscrtavanja
		SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
	}
	void clear(boja boja) {				//Iscrtavanje prazne površine
		crtajBojom(boja);
		SDL_RenderClear(frameBuffer);
	}

	void odgoda(int djeljiteljSekunde) {		//Funkcija vremenske odgode za periode manje od jedne sekunde, 
		SDL_Delay(1000 / djeljiteljSekunde);	//prima vrijednost kojom je izraženo za koliko je puta odgoda manja od jedne sekunde
	}

	void commit() {						//Funkcija slanja spremnika sa elementima spremnika u kojem su 
		SDL_RenderPresent(frameBuffer);	//sadržani svi grafièki elementi koji se iscrtavaju u jednom okviru po sekundi
	}
};

enum Key {				//enumeracijska struktura u kojoj su poredane cjelobrojne vrijednosti tipki tipkovnice 
	NISTA = 0,			//koje se predaju funkcijama biblioteke za upravljanje grafièkim elementima na zaslonu
	RAZMAKNICA = 41,
	GORE = 82,
	DOLJE = 81
};

class Pojava {			//Suèelje pojava koje se pojavljuju na zaslonu u kojem su sadržana svojsta 
public:					//za implementaciju parametara i funkcija potrebnih za izvoðenje ponašanja
	virtual void smjer(Key K) = 0;		//Deklaracija funkcije smjera koja prima vrijednost tipke za okidanje odreðene reakcije pojave
	virtual void kretanje(){}			//Deklaracija funkcije kretanja pojave
	void postaviStanje(Key K) {			//Funkcija koja prosljeðuje vrijednost pritisnute tipke tipkovnice pojavi
		stanjeTipke = K;
	}
	
	Key stanjeTipke;

	int x, y;			//Svojstva položaja koordinata poèetne toèke iscrtavanje pojave [px]
	int w, h;			//Svojstva širine i visine pojave za iscrtavanje [px]
	int dy;				//Svojstvo iznosa vertikalnog pomaka pojave [px]
};

class Igrac : public Pojava {		//Razred izveden od razreda pojave, moguæe ga je upravljati tipkovnicom
public:
	void postaviPocetneDimenzije(Display* d) {		//Funkcija konstrukcije osnovnih parametara iscrtavanja pojave
		w = 20; h = 100;
		x = 0; y = d->Visina / 2 - h / 2;
		smjer(NISTA);
		pogodak = 0;
	}
	void postaviStanje(Key K) {		//Funkcija koja prosljeðuje vrijednost pritisnute tipke tipkovnice pojavi
		stanjeTipke = K;
	}
	void smjer(Key K) {				//Funkcija upravljanja smjerom kretanja pomoæu tipkovnice
		if (K == GORE) {
			dy = -10;
		}
		else if (K == DOLJE) {
			dy = 10;
		}
		else dy = 0;
	}
	void kretanje(Display* ptrZaslon) {		//Funkcija kretanja pojave u podjeli vremena
		if (((y > 0) && ((y + h) < ptrZaslon->Visina))	//Ako je poèetna toèka iscrtavanja pojave niža od vrha zaslona ili završna toèka viša od dna
			|| (y <= 0 && dy > 0)						//ili ako je poèetna toèka viša od vrha i kretanje prema dnu
			|| (y + h >= ptrZaslon->Visina && dy < 0))	//ili završna toèka viša od dna i kretanje prema vrhu
			y += dy;									//pojava 'Igrac' kreæe se u proizvoljno smjeru smjeru
		smjer(stanjeTipke);		//Smjer je reguliran u skladu sa pritisnutom tipkom
	}
	void uvecajPogodak() {	//Funkcija uveæavanja pogotka igraèa
		pogodak += 1;
	}

	int pogodak;			//Svojstvo pohrane postignutih pogodaka
};

class InputListener			//Suèelje za listener razrede u skladu sa principima Observer oblikovnog obrasca
{
public:
	
	virtual void dodajPojavu(Igrac* p) = 0;	//Virtualna funkcija za implementaciju dodavanja pojave koje izvedeni Listener prati
	virtual void buttonPressed(Key K) = 0;	//implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi pritisak tipke
	virtual void buttonReleased(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi otpuštanje tipke
};

class Input					//Suèelje za izvedene razrede praæenja elemenata u tijeku izvoðenja, u skladu sa principima Observer oblikovnog obrasca
{
public:
	virtual void addListener(InputListener* listener) = 0;		//Virtualna funkcija za implementaciju dodavanja listenera koji prate dogaðaje povezane s elementima implementacije
	virtual void removeListener(InputListener* listener) = 0;	//Virtualna funkcija za implementaciju uklanjanja listenera koji prate dogaðaje povezane s elementima implementacije

	list<InputListener*> listeners;			//Lista Listenera koji prate dogaðaje povezane s elementima izvoðenja
	bool pogon;								//Flag istinitosti izvoðenja igre
};


class Loptica : public Pojava {		//Razred izveden od razreda pojave, predstavlja tijelo koje se odbija od pojava u igri u svrhu postizanja pogodaka
public:
	int dx;					//Za razliku od ostalih pojava, 'Loptica' sadrži horizontalnu komponentu putanje pohranjenu u ovom svojstvu [px]

	void postaviPocetneDimenzije(Display* ptrZaslon) {		//Funkcija konstrukcije osnovnih parametara iscrtavanja pojave
		x = ptrZaslon->Sirina / 2, y = rand() % ptrZaslon->Visina;
		w = 20; h = 20; promjenaDinamikeKretanja(-3);
	}
	void smjer(Display ptrZaslon, Key K) {}
	void smjer(Display* ptrZaslon) {													//Funkcija regulacije smjera loptice
		if ((y >= ptrZaslon->Visina - h)|| (y <= 0)) { dy *= -1; }						//Ako loptica dosegne dno ili vrh odbija se u suprotnom smjeru,
		else if (x >= ptrZaslon->Sirina) { postaviPocetneDimenzije(ptrZaslon); }		//osim ako prelazi širinu zaslona, kada se vraæa u poèetni položaj
	}
	void smjer(Key K) {}
	void kretanje(Display* ptrZaslon) {		//Funkcija kretanja pojave u podjeli vremena
		x += dx;
		y += dy;
		smjer(ptrZaslon);					//Smjer je reguliran u skladu sa zadanim postavkama kretanja u definiciji funkcije
	}
	void promjenaDinamikeKretanja(int px) {
		if (dy == 0 || dx == 0) dy = px, dx = px;
		else {
			dy = dy / abs(dy) * px;
			dx = dx / abs(dx) * px;
		}
	}
};

class Protivnik : public Pojava {			//Razred izveden od razreda pojave, upravlja se autonomno u skladu sa zadanim uvjetima
public:
	void postaviPocetneDimenzije(Display* ptrZaslon, Loptica* ptrLoptica) {				//Funkcija konstrukcije osnovnih parametara iscrtavanja pojave
		pogodak = 0;
		w = 20; h = 100;
		x = ptrZaslon->Sirina - 20;
		y = ptrZaslon->Visina / 2 - h / 2;
		promjenaDinamikeKretanja(3);
		if (ptrLoptica->dy < 0) { dy *= -1; }
	}
	
	void smjer(Key K){}			//nadjaèavanje virtualne funkcije iz suèelja 'Pojava' praznom implementacijom 
	void smjer(Display* ptrZaslon, Loptica* ptrLoptica) {
		okretanjeSmjeraProtivnika(ptrLoptica);
		zadrzavanjeProtivnikaUnutarRubova(ptrZaslon);
		smanjenjeBrzineProtivnika(ptrLoptica);
	}
	void kretanje(Display* ptrZaslon, Loptica* ptrLoptica) {		//Funkcija kretanja pojave u podjeli vremena
		y += dy;
		smjer(ptrZaslon, ptrLoptica);								//Smjer je reguliran u skladu sa zadanim postavkama kretanja u definiciji funkcije
	}
	void uvecajPogodak() {		//Funkcija uveæavanja pogotka igraèa
		pogodak += 1;
	}
	void okretanjeSmjeraProtivnika(Loptica* ptrLoptica) {			//
		if (lopticaSeKrecePremaDolje(ptrLoptica)) promjenaDinamikeKretanja(5);			//Okreni pojavu 'Protivnik' u smjeru loptice prema dolje
		else if (lopticaSeKrecePremaGore(ptrLoptica)) promjenaDinamikeKretanja(-5);		//Okreni pojavu 'Protivnik' u smjeru loptice prema gore
	}
	void zadrzavanjeProtivnikaUnutarRubova(Display* ptrZaslon) {			//
		if (protivnikJeNaDnu(ptrZaslon)) y = 0;								//Ako je pojava 'Protivnik' došla do vrha terena zaustavi je							
		else if (protivnikJeNaVrhu(ptrZaslon)) y = ptrZaslon->Visina - h;	//Ako je pojava 'Protivnik došla do dna terena zaustavi je
	}
	void smanjenjeBrzineProtivnika(Loptica* ptrLoptica) {									//
		if (lopticaSeKreceOdProtivnika(ptrLoptica)) (ptrLoptica->dy > 0) ? promjenaDinamikeKretanja(2) : promjenaDinamikeKretanja(-2);		//Ako loptica ide u smjeru pojave igrac smanji brzinu kretanja pojave 'Protivnik'
	}

	void promjenaDinamikeKretanja(int px) {
		dy = px;
	}

	bool lopticaSeKrecePremaDolje(Loptica* ptrLoptica) { return (ptrLoptica->y + ptrLoptica->h) / 2 > (y + h) / 2; }
	bool lopticaSeKrecePremaGore(Loptica* ptrLoptica) { return (ptrLoptica->y + ptrLoptica->h) / 2 < (y + h) / 2; }
	bool protivnikJeNaDnu(Display* ptrZaslon) { return y <= 0; }
	bool protivnikJeNaVrhu(Display* ptrZaslon) { return (y + h) >= ptrZaslon->Visina; }
	bool lopticaSeKreceOdProtivnika(Loptica* ptrLoptica) {return ptrLoptica->dx < 0;
}

	int pogodak;				//Svojstvo pohrane postignutih pogodaka
};

class KeyListener : public InputListener {		//Konkretna implementacija suèelja InputListener, za Listenere dogaðaja pritiska tipki tipkovnice, u skladu sa principima Observer oblikovnog obrasca
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

	Igrac* opPojava;							//Pojava èije se ponašanje mijenja
};

class KeyboardInput : public Input {		//Razred izveden od suèelja 'Input', za bilježenje dogaðaja pritiska tipke i aktivaciju zadanih funkcijskih poziva,
public:										//u skladu sa principima Observer oblikovnog obrasca
	void updateInput() {		//Funkcija bilježenja dogaðaja i slanja bilješke registriranim Listenerima
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:					//Sluèaj slanja obavijesti da je tipka pritisnuta
				updateListeners(event);
				break;
			case SDL_KEYUP:						//Sluèaj slanja obavijesti da nije tipka pritisnuta
				updateListeners(event);
				break;
			case SDL_QUIT: pogon = false;		//Sluèaj slanja obavijsti da igra više nije u pogonu
			default:
				pogon = true;
				break;
			}
		}
	}
	void addListener(InputListener* listener) { kLst.push_back((KeyListener*)listener); }		//Funkcija registracije Listenera
	void removeListener(InputListener* listener) { kLst.remove((KeyListener*)listener); }		//Funkcija deregistracije Listenera
	void updateListeners(SDL_Event event) {		//Funkcija slanja obavijesti registriranim Listenerima
		for (KeyListener* l : kLst) {
			if (event.type == SDL_KEYDOWN) {										//U trenutku pritiska tipke
				switch (event.key.keysym.scancode) {								//bilježi dogaðaj na sljedeæi naèin:
				case SDL_SCANCODE_SPACE: l->buttonPressed(RAZMAKNICA); break;		//ako je pritisnuta razmaknica,
				case SDL_SCANCODE_UP: l->buttonPressed(GORE); break;				//ako je pritisnuta navigacijska tipka 'gore',
				case SDL_SCANCODE_DOWN: l->buttonPressed(DOLJE); break;				//ako je pritisnuta navigacijska tipka 'dolje,
				default: break;														//ako tipka nije pritisnuta
				}
			}
			else if (event.type == SDL_KEYUP) {										
				l->buttonReleased(NISTA);											//i ako je tipka otpuštena
			}
		}
	}

	list<KeyListener*> kLst;					//Lista registriranih listenera
	SDL_Event event;							//Unija koja sadržava svojstva dogaðaja definiranog u zadanoj biblioteci
};








