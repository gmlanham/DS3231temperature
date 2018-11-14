#ifndef temp3231_h
#define temp3231_h

// Changed the following to work on 1.0
//#include "WProgram.h"
#include <Arduino.h>

#include <Wire.h>

class temp3231 {
	public:
			
		//Constructor
		temp3231();
		
		// Temperature function
		float getTemperature(); 
		
	private:
	
};

#endif
