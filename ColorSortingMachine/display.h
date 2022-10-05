//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setupDisplay()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Color Sorting");
  lcd.setCursor(0,1);
  lcd.print("    Machine");
}


void displayCounter(int zaehler[6])
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zaehlerstand:");
  lcd.setCursor(0, 1);
  lcd.print(zaehler[1], true);
  lcd.print(" ");
  lcd.print(zaehler[2], true);
  lcd.print(" ");
  lcd.print(zaehler[3], true);
  lcd.print(" ");
  lcd.print(zaehler[4], true);
  lcd.print(" ");
  lcd.print(zaehler[5], true);
  lcd.print(" ");
  
}
