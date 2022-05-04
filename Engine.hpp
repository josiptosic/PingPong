#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <list>
#include <cmath>

using namespace std;

class Display							//Razred Display koji sadr�i funkcijska i podatkovna svojstva zaslona za iscrtavanje
{
private:
	SDL_Window* Prozor;					//Deklaracija pokaziva�a na strokturu koja sadr�i svojstva stvorenog prozora
	Uint32 render_flags;				//Flagovi uputa iscrtavanja
	SDL_Renderer* frameBuffer;			//Deklaracija pokaziva�a na strukturu koja sadr�i svojstva spremnika za iscrtavanje
	TTF_Font* Font;						//Deklaracija pokaziva�a a strukturu koja sadr�i svojstva fonta za iscrtani tekst
	SDL_Color Bijela;					//Deklaracija strukture koja sad�i svojstva boje za iscrtavanje teksta
	SDL_Surface* surfaceMessage;		//Deklaracija pokaziva�a na strukturu koja sad�i svojstva povr�ine za iscrtavanje
	SDL_Texture* Message;				//Deklaracija pokaziva�a na strukturu koja sad�i svojstva poruke koja se iscrtava kao tekst
	SDL_Rect Message_rect;				//Deklaracija pravokutnika koji sadr�i tekst poruke za iscrtavanje
	SDL_Rect pravokutnik;				//Deklaracija strukture koja sadr�i svojstva za iscrtavanje pravokutnika

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

	int Sirina;							//�irina zaslona za iscrtavanje izra�ena u pikselima
	int Visina;							//Visina zaslona za iscrtavanje izra�ena u pikselima

	struct boja {						//Struktura koja sadr�i RGB parametre boje za iscrtavanje
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
		if (!Prozor) std::cout << "Dogodila se gre�ka prilikom stvaranja prozora!" << std::endl;
		frameBuffer = SDL_CreateRenderer(Prozor, -1, render_flags);
		if (!frameBuffer) { std::cout << "Dogodila se gre�ka prilikom stvaranja frameBuffera!" << std::endl; SDL_DestroyWindow(Prozor); }
	}
	void crtajLiniju(int xP, int yP, int xZ, int yZ, boja b) {						//Funkcija iscrtavanja linije
		SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
		SDL_RenderDrawLine(frameBuffer, xP, yP, xZ, yZ);
	}
	void crtajPravokutnik(int polozajX, int polozajY, int sirina, int visina, boja b) {	//Funkcija iscrtavanja pravokutnika
		
		pravokutnik.x = polozajX;		//Koordinata po�etne to�ke iscrtavanja na X osi
		pravokutnik.y = polozajY;		//Koordinata po�etne to�ke iscrtavanja na Y osi
		pravokutnik.w = sirina;			//�irina pravokutnika za iscrtavanje, izra�ena u pikselima
		pravokutnik.h = visina;			//Visina pravokutnika za iscrtavanje, izra�ena u pikselima
		crtajBojom(b);
		
		SDL_RenderDrawRect(frameBuffer, &pravokutnik);
		SDL_RenderFillRect(frameBuffer, &pravokutnik);
	}
	void crtajTekst(string poruka, int x, int y){		//Funkcija iscrtavanja teksta na zaslon
		Bijela = { 255, 255, 255 };
		surfaceMessage = TTF_RenderText_Solid(Font, poruka.c_str(), Bijela);
		Message = SDL_CreateTextureFromSurface(frameBuffer, surfaceMessage);

		Message_rect.x = x;				//Koordinata po�etne to�ke iscrtavanja na X osi
		Message_rect.y = y;				//Koordinata po�etne to�ke iscrtavanja na Y osi
		Message_rect.w = 15 * strlen(poruka.c_str());		//�irina pravokutnika za iscrtavanje teksta, izra�ena u pikselima
		Message_rect.h = 50;			//Visina pravokutnika za iscrtavanje teksta, izra�ena u pikselima

		SDL_RenderCopy(frameBuffer, Message, NULL, &Message_rect);
		SDL_RenderDrawRect(frameBuffer, &Message_rect);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
	}
	void crtajBojom(boja b) {			//Boja iscrtavanja
		SDL_SetRenderDrawColor(frameBuffer, b.r, b.g, b.b, 255);
	}
	void clear(boja boja) {				//Iscrtavanje prazne povr�ine
		crtajBojom(boja);
		SDL_RenderClear(frameBuffer);
	}

	void odgoda(int djeljiteljSekunde) {		//Funkcija vremenske odgode za periode manje od jedne sekunde, 
		SDL_Delay(1000 / djeljiteljSekunde);	//prima vrijednost kojom je izra�eno za koliko je puta odgoda manja od jedne sekunde
	}

	void commit() {						//Funkcija slanja spremnika sa elementima spremnika u kojem su 
		SDL_RenderPresent(frameBuffer);	//sadr�ani svi grafi�ki elementi koji se iscrtavaju u jednom okviru po sekundi
	}
};

enum Key {				//enumeracijska struktura u kojoj su poredane cjelobrojne vrijednosti tipki tipkovnice 
	NISTA = 0,			//koje se predaju funkcijama biblioteke za upravljanje grafi�kim elementima na zaslonu
	RAZMAKNICA = 41,
	GORE = 82,
	DOLJE = 81
};

class Pojava {			//Su�elje pojava koje se pojavljuju na zaslonu u kojem su sadr�ana svojsta 
public:					//za implementaciju parametara i funkcija potrebnih za izvo�enje pona�anja
	virtual void smjer(Key K) = 0;		//Deklaracija funkcije smjera koja prima vrijednost tipke za okidanje odre�ene reakcije pojave
	virtual void kretanje(){}			//Deklaracija funkcije kretanja pojave
	void postaviStanje(Key K) {			//Funkcija koja proslje�uje vrijednost pritisnute tipke tipkovnice pojavi
		stanjeTipke = K;
	}
	
	Key stanjeTipke;

	int x, y;			//Svojstva polo�aja koordinata po�etne to�ke iscrtavanje pojave [px]
	int w, h;			//Svojstva �irine i visine pojave za iscrtavanje [px]
	int dy;				//Svojstvo iznosa vertikalnog pomaka pojave [px]
};

class Igrac : public Pojava {		//Razred izveden od razreda pojave, mogu�e ga je upravljati tipkovnicom
public:
	void postaviPocetneDimenzije(Display* d) {		//Funkcija konstrukcije osnovnih parametara iscrtavanja pojave
		w = 20; h = 100;
		x = 0; y = d->Visina / 2 - h / 2;
		smjer(NISTA);
		pogodak = 0;
	}
	void postaviStanje(Key K) {		//Funkcija koja proslje�uje vrijednost pritisnute tipke tipkovnice pojavi
		stanjeTipke = K;
	}
	void smjer(Key K) {				//Funkcija upravljanja smjerom kretanja pomo�u tipkovnice
		if (K == GORE) {
			dy = -10;
		}
		else if (K == DOLJE) {
			dy = 10;
		}
		else dy = 0;
	}
	void kretanje(Display* ptrZaslon) {		//Funkcija kretanja pojave u podjeli vremena
		if (((y > 0) && ((y + h) < ptrZaslon->Visina))	//Ako je po�etna to�ka iscrtavanja pojave ni�a od vrha zaslona ili zavr�na to�ka vi�a od dna
			|| (y <= 0 && dy > 0)						//ili ako je po�etna to�ka vi�a od vrha i kretanje prema dnu
			|| (y + h >= ptrZaslon->Visina && dy < 0))	//ili zavr�na to�ka vi�a od dna i kretanje prema vrhu
			y += dy;									//pojava 'Igrac' kre�e se u proizvoljno smjeru smjeru
		smjer(stanjeTipke);		//Smjer je reguliran u skladu sa pritisnutom tipkom
	}
	void uvecajPogodak() {	//Funkcija uve�avanja pogotka igra�a
		pogodak += 1;
	}

	int pogodak;			//Svojstvo pohrane postignutih pogodaka
};

class InputListener			//Su�elje za listener razrede u skladu sa principima Observer oblikovnog obrasca
{
public:
	
	virtual void dodajPojavu(Igrac* p) = 0;	//Virtualna funkcija za implementaciju dodavanja pojave koje izvedeni Listener prati
	virtual void buttonPressed(Key K) = 0;	//implementacije Input razreda �e otpu�tati ovu funkciju kada se dogodi pritisak tipke
	virtual void buttonReleased(Key K) = 0; //implementacije Input razreda �e otpu�tati ovu funkciju kada se dogodi otpu�tanje tipke
};

class Input					//Su�elje za izvedene razrede pra�enja elemenata u tijeku izvo�enja, u skladu sa principima Observer oblikovnog obrasca
{
public:
	virtual void addListener(InputListener* listener) = 0;		//Virtualna funkcija za implementaciju dodavanja listenera koji prate doga�aje povezane s elementima implementacije
	virtual void removeListener(InputListener* listener) = 0;	//Virtualna funkcija za implementaciju uklanjanja listenera koji prate doga�aje povezane s elementima implementacije

	list<InputListener*> listeners;			//Lista Listenera koji prate doga�aje povezane s elementima izvo�enja
	bool pogon;								//Flag istinitosti izvo�enja igre
};


class Loptica : public Pojava {		//Razred izveden od razreda pojave, predstavlja tijelo koje se odbija od pojava u igri u svrhu postizanja pogodaka
public:
	int dx;					//Za razliku od ostalih pojava, 'Loptica' sadr�i horizontalnu komponentu putanje pohranjenu u ovom svojstvu [px]

	void postaviPocetneDimenzije(Display* ptrZaslon) {		//Funkcija konstrukcije osnovnih parametara iscrtavanja pojave
		x = ptrZaslon->Sirina / 2, y = rand() % ptrZaslon->Visina;
		w = 20; h = 20; promjenaDinamikeKretanja(-3);
	}
	void smjer(Display ptrZaslon, Key K) {}
	void smjer(Display* ptrZaslon) {													//Funkcija regulacije smjera loptice
		if ((y >= ptrZaslon->Visina - h)|| (y <= 0)) { dy *= -1; }						//Ako loptica dosegne dno ili vrh odbija se u suprotnom smjeru,
		else if (x >= ptrZaslon->Sirina) { postaviPocetneDimenzije(ptrZaslon); }		//osim ako prelazi �irinu zaslona, kada se vra�a u po�etni polo�aj
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
	
	void smjer(Key K){}			//nadja�avanje virtualne funkcije iz su�elja 'Pojava' praznom implementacijom 
	void smjer(Display* ptrZaslon, Loptica* ptrLoptica) {
		okretanjeSmjeraProtivnika(ptrLoptica);
		zadrzavanjeProtivnikaUnutarRubova(ptrZaslon);
		smanjenjeBrzineProtivnika(ptrLoptica);
	}
	void kretanje(Display* ptrZaslon, Loptica* ptrLoptica) {		//Funkcija kretanja pojave u podjeli vremena
		y += dy;
		smjer(ptrZaslon, ptrLoptica);								//Smjer je reguliran u skladu sa zadanim postavkama kretanja u definiciji funkcije
	}
	void uvecajPogodak() {		//Funkcija uve�avanja pogotka igra�a
		pogodak += 1;
	}
	void okretanjeSmjeraProtivnika(Loptica* ptrLoptica) {			//
		if (lopticaSeKrecePremaDolje(ptrLoptica)) promjenaDinamikeKretanja(5);			//Okreni pojavu 'Protivnik' u smjeru loptice prema dolje
		else if (lopticaSeKrecePremaGore(ptrLoptica)) promjenaDinamikeKretanja(-5);		//Okreni pojavu 'Protivnik' u smjeru loptice prema gore
	}
	void zadrzavanjeProtivnikaUnutarRubova(Display* ptrZaslon) {			//
		if (protivnikJeNaDnu(ptrZaslon)) y = 0;								//Ako je pojava 'Protivnik' do�la do vrha terena zaustavi je							
		else if (protivnikJeNaVrhu(ptrZaslon)) y = ptrZaslon->Visina - h;	//Ako je pojava 'Protivnik do�la do dna terena zaustavi je
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

class KeyListener : public InputListener {		//Konkretna implementacija su�elja InputListener, za Listenere doga�aja pritiska tipki tipkovnice, u skladu sa principima Observer oblikovnog obrasca
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

	Igrac* opPojava;							//Pojava �ije se pona�anje mijenja
};

class KeyboardInput : public Input {		//Razred izveden od su�elja 'Input', za bilje�enje doga�aja pritiska tipke i aktivaciju zadanih funkcijskih poziva,
public:										//u skladu sa principima Observer oblikovnog obrasca
	void updateInput() {		//Funkcija bilje�enja doga�aja i slanja bilje�ke registriranim Listenerima
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:					//Slu�aj slanja obavijesti da je tipka pritisnuta
				updateListeners(event);
				break;
			case SDL_KEYUP:						//Slu�aj slanja obavijesti da nije tipka pritisnuta
				updateListeners(event);
				break;
			case SDL_QUIT: pogon = false;		//Slu�aj slanja obavijsti da igra vi�e nije u pogonu
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
				switch (event.key.keysym.scancode) {								//bilje�i doga�aj na sljede�i na�in:
				case SDL_SCANCODE_SPACE: l->buttonPressed(RAZMAKNICA); break;		//ako je pritisnuta razmaknica,
				case SDL_SCANCODE_UP: l->buttonPressed(GORE); break;				//ako je pritisnuta navigacijska tipka 'gore',
				case SDL_SCANCODE_DOWN: l->buttonPressed(DOLJE); break;				//ako je pritisnuta navigacijska tipka 'dolje,
				default: break;														//ako tipka nije pritisnuta
				}
			}
			else if (event.type == SDL_KEYUP) {										
				l->buttonReleased(NISTA);											//i ako je tipka otpu�tena
			}
		}
	}

	list<KeyListener*> kLst;					//Lista registriranih listenera
	SDL_Event event;							//Unija koja sadr�ava svojstva doga�aja definiranog u zadanoj biblioteci
};








