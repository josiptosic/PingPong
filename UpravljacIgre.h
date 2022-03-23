#include "Display.h"
#include "Input.h"
#include "Pojava.h"

#pragma once

namespace CircuitMessPong {

	class UpravljacIgre
	{
		UpravljacIgre(Display d) {
			KeyListener kListener();
			KeyboardInput kInput();
			kInput().listeners;
			kInput().addListener(kListener);
			
			
			
			Igrac i1(d);
			Loptica l(d);
			Protivnik i2(d, l);
			d.crtajBojom(d.tamnozelena);
			d.crtajPravokutnik(i1.x, i1.y, i1.w, i1.h, d.bijela);
			d.crtajPravokutnik(l.x, l.y, l.w, l.h, d.zuta);
			d.crtajPravokutnik(i2.x, i2.y, i2.w, i2.h, d.bijela);
			
		}

		
		
	};

	


}