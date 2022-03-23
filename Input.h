#include <list>
#include <SDL2/SDL.h>
#include <iostream>


#pragma once

using namespace std;

namespace CircuitMessPong {






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

		//void buttonPressed(Key K) {}
		//void buttonReleased(Key K) {}

		virtual void buttonPressed(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi pritisak tipke
		virtual void buttonReleased(Key K) = 0; //implementacije Input razreda æe otpuštati ovu funkciju kada se dogodi otpuštanje tipke

	};







	class Input
	{	

	public:

		virtual void addListener(InputListener* listener) = 0;

		virtual void removeListener(InputListener* listener) = 0;

		SDL_Event e;

	};





	
	class KeyListener : public InputListener {

	public:
		KeyListener() {

		}

		void buttonPressed(Key K) {
		
			kState = K;
		
		}
		void buttonReleased(Key K) {
		
			kState = NISTA;

		}

		Key kState;

		void update() {

		}
	};






	class KeyboardInput : public Input {
	public: 
		list<KeyListener*> listeners;
		int pritisnutaTipka;
		KeyListener l;
		KeyboardInput() {
			
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_KEYDOWN:
					updateListeners();
					break;
				case SDL_KEYUP:
					updateListeners();
					break;
				default:
					break;
				}
			}
		}

		void addListener(InputListener* listener) { listeners.push_back(listener); }
		void removeListener(InputListener* listener) { listeners.remove(listener); }
		void updateListeners() {
			for (InputListener* l : listeners) {
				if (e.key.keysym.scancode == RAZMAKNICA) l->buttonPressed(RAZMAKNICA);
				if (e.key.keysym.scancode == GORE) l->buttonPressed(GORE);
				if (e.key.keysym.scancode == DOLJE)l->buttonPressed(DOLJE);
			}
		}


	};
	//e.key.keysym.scancode 

}