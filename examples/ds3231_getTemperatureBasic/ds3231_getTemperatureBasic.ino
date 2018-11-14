//#path "D:\Users\mike\Documents\Arduino\libraries\temp3231\examples\ds3231_getTemperatureBasic\temp3231.h"
#include <temp3231.h> 
temp3231 RTC;
int tempC;
int tempF;
 
void setup() {
  //RTC.begin();
}
 
void loop() {
  tempC = RTC.getTemperature();
  tempF = (tempC * 1.8) + 32.0; // Convert C to F
 
  //lcd.print(tempF);
  //lcd.print("F");
}
