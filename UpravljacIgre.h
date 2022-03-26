#pragma once

#include "SDL2/SDL.h"
#include "Engine.hpp"

	class UpravljacIgre
	{
	public:
		UpravljacIgre(Display d) {
			Igrac i1(d);
			Loptica l(d);
			Protivnik i2(d, l);
			KeyboardInput* kInput = new KeyboardInput;
			KeyListener* kL = new KeyListener;
			kInput->addListener(kL);
			//kInput();
			//kInput();
			//kInput().kLst;
			//kInput().e;
			//kInput().addListener((InputListener*)kListener);
			//kInput->updateInput();
			kL->dodajPojavu(&i1);		
			kInput->e;
			kInput->pogon = true;
			while (kInput->pogon==true) {
				
				kInput->updateInput();
				
				i1.kretanje();
				l.kretanje();
				i2.kretanje();
				
				
				d.clear(d.bijela);
				d.crtajBojom(d.tamnozelena);
				d.crtajPravokutnik(i1.x, i1.y, i1.w, i1.h, d.bijela);
				d.crtajPravokutnik(l.x, l.y, l.w, l.h, d.zuta);
				d.crtajPravokutnik(i2.x, i2.y, i2.w, i2.h, d.bijela);
				d.commit();
			}	
		}
	};

	


