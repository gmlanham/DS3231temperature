/*This sketch uses the DS3231 RTC to display date, time and temperature on
 * an LCD. Three buttons are used to set the date and time.
 */
//************libraries**************//
#include <Bounce2.h>
#include <Wire.h>
#include <RTClib-Temperature.h>
#include <LiquidCrystal.h>

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_DS3231 RTC;

//************Button*****************//
int P1=6; // Button SET MENU'
int P2=7; // Button +
int P3=8; // Button -

#define LED_PIN 13
const uint8_t BUTTON_PINS[]  = {P1, P2, P3};//array of buttons
#define NUM_BUTTONS sizeof(BUTTON_PINS)
#define DS3231_I2C_ADDR             0x68
#define DS3231_TEMPERATURE_ADDR     0x11
short temp;
int ledState = LOW;
int d = 100; //debounce delay

Bounce * buttons = new Bounce[NUM_BUTTONS];

//************Variables**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;

void setup()
{
  lcd.begin(16,2);
  lcd.clear();

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(10);              // interval in ms
  }
  
  Serial.begin(9600);
  delay(d);  
  while (!Serial);
  delay(d);
  Serial.println();
  Serial.println(__DATE__);  
  Serial.println(__TIME__);
  Serial.println(__FILE__);
  Serial.println();
  Serial.flush();

  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time

  // The default display shows the date and time
  int menu=0;
}

void ledToggle(){
  bool needToToggleLed = false;
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // Update the Bounce instance :
    buttons[i].update();
    // If it fell, flag the need to toggle the LED
    if ( buttons[i].fell() ) {
      needToToggleLed = true;
    }
  }

  // if a LED toggle has been flagged :
  if ( needToToggleLed ) {
    // Toggle LED state :
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    delay(d);
  }
}

void loop()
{ 
  //delay(d);  
  //int tempC = DS3231_get_treg();  // Reads the temperature as an int, to save memory
  int tempC = RTC.getTemperature(true);
  //delay(d*100);
  //Serial.println(tempC);
  //Serial.println(tempC*1.8+32,1);
  //lcd.clear();

  //show temperature update each 1 sec.
  lcd.setCursor(0, 0);
  lcd.print("                "); 
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");   
  lcd.print(tempC*1.8+32,1); lcd.print("F    ");  
  delay(d*10);  
  ledToggle();
  //delay(d);
  
// check if you press the SET button and increase the menu index
  if(digitalRead(P1))
  {
   menu=menu+1;
  }
  
// in which subroutine should we go?
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
      //delay(d*10);        
    }
  if (menu==1)
    {
    DisplaySetHour();          
    }
  if (menu==2)
    {
    DisplaySetMinute();
      //delay(d);
    }
  if (menu==3)
    {
    DisplaySetYear();
     //delay(d);
    }
  if (menu==4)
    {
    DisplaySetMonth();
     // delay(d);
    }
  if (menu==5)
    {
    DisplaySetDay();
      //delay(d);
    }
  if (menu==6)
    {
    StoreAgg(); 
    //delay(d);
    menu=0;
    }
    //delay(d);
}

void DisplayDateTime ()
{
// We show the current date and time
  DateTime now = RTC.now();
  //show time for 5sec.
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);
  lcd.print("Hour: ");
  if (now.hour()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC); 
  hourupg=now.hour();
  lcd.print(":");
  if (now.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);  
  minupg=now.minute();
  lcd.print(":");
  if (now.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);    
  delay(d*50);
  
  //show date for 0.5sec.
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  if (now.day()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC); 
  dayupg=now.day();
  lcd.print("/");
  if (now.month()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  monthupg=now.month();
  lcd.print("/");
  lcd.print(now.year(), DEC); 
  yearupg=now.year();
  delay(d*5);
}

void DisplaySetHour()
{
// time setting
  lcd.clear();
  DateTime now = RTC.now();
  String I = String(digitalRead(P2)); 
  if(digitalRead(P2)==HIGH)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
   
  }
   if(digitalRead(P3)==HIGH)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
    
  }
  lcd.setCursor(0,0);
  lcd.print("Set time:");
  lcd.setCursor(0,1);
  lcd.print(hourupg,DEC); 
  delay(d);
}

void DisplaySetMinute()
{
// Setting the minutes
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (minupg==0)
    {
      minupg=59;
    }
    else
    {
      minupg=minupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minupg,DEC); 
  delay(d);
}
  
void DisplaySetYear()
{
// setting the year
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {    
    yearupg=yearupg+1;
  }
   if(digitalRead(P3)==HIGH)
  {
    yearupg=yearupg-1;
  }
  lcd.setCursor(0,0);
  lcd.print("Set Year:");
  lcd.setCursor(0,1);
  lcd.print(yearupg,DEC);
  delay(d);
}

void DisplaySetMonth()
{
// Setting the month
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (monthupg==12)
    {
      monthupg=1;
    }
    else
    {
      monthupg=monthupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (monthupg==1)
    {
      monthupg=12;
    }
    else
    {
      monthupg=monthupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  lcd.print(monthupg,DEC);
  delay(d);
}

void DisplaySetDay()
{
// Setting the day
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (dayupg==31)
    {
      dayupg=1;
    }
    else
    {
      dayupg=dayupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (dayupg==1)
    {
      dayupg=31;
    }
    else
    {
      dayupg=dayupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Day:");
  lcd.setCursor(0,1);
  lcd.print(dayupg,DEC);
  delay(d);
}

void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
  delay(d);
}
