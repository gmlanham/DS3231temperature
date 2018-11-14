#include <temp3231.h>

// These included for the DateTime class inclusion; will try to find a way to
// not need them in the future...
#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
// Changed the following to work on 1.0
//#include "WProgram.h"
#include <Arduino.h>

// Constructor
temp3231::temp3231() {
	// nothing to do for this constructor.
}

float temp3231::getTemperature() {
	// Checks the internal thermometer on the DS3231 and returns the 
	// temperature as a floating-point value.

  // Updated / modified a tiny bit from "Coding Badly" and "Tri-Again"
  // http://forum.arduino.cc/index.php/topic,22301.0.html
  #define CLOCK_ADDRESS 0x68  
  byte tMSB, tLSB;
  float temp3231;
  
  // temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 2);

  // Should I do more "if available" checks here?
  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
  }
  else {
    temp3231 = -9999; // Some obvious error value
  }
   
  return temp3231;
}
