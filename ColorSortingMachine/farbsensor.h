

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include "ColorName.h"

#define SENSOR_LIGHT_PIN A3

// Change this to based on whether you are using a common anode or common cathode
// RGB LED. See: https://makeabilitylab.github.io/physcomp/arduino/rgb-led
// If you are working with a common cathode RGB LED, set this to false.
// Note Adafruit sells a common anode design: https://www.adafruit.com/product/159
// but for our courses, we try to purchase common cathodes (as they're more straightforward
// to use).
const boolean COMMON_ANODE = false;
const int DELAY_MS = 200;
const int RGB_RED_PIN = 5;
const int RGB_GREEN_PIN = 6;
const int RGB_BLUE_PIN = 11;
const int MAX_COLOR_VALUE = 255;
const boolean USE_GAMMA_CORRECTION = true;

Adafruit_TCS34725 _colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// our RGB -> eye-recognized gamma color
// See: https://learn.adafruit.com/chameleon-scarf/code
byte _gammaTable[256];



// Raw values from https://www.rapidtables.com/web/color/RGB_Color.html
//const int NUM_COLOR_NAMES = 8;
//ColorName _colorNames[NUM_COLOR_NAMES] = {
//  ColorName("Red", 0xFF, 0x00, 0x00),
//  ColorName("Orange", 0xFF, 0xA5, 0x00),
//  ColorName("Yellow", 0xFF, 0xFF, 0x00),
//  ColorName("Green", 0x00, 0xFF, 0x00),
//  ColorName("Blue", 0x00, 0x00, 0xFF),
//  ColorName("Cyan", 0x00, 0xFF, 0xFF),
//  ColorName("Magenta", 0xFF, 0x00, 0xFF),
//  ColorName("Purple", 0x80, 0x00, 0x80)
//};

// Measured values. Change these according to the values measured
// and printed out via Serial.print to the screen
const int NUM_COLOR_NAMES = 6;
ColorName _colorNames[NUM_COLOR_NAMES] = {
  ColorName("None", 0, 115, 85, 60),
  ColorName("Red", 1, 193, 49, 44),
  ColorName("Orange", 2, 159, 65, 38),
  ColorName("Yellow", 3, 119, 96, 41),
  ColorName("Green", 4, 88, 123, 50),
  ColorName("Purple", 5, 150, 74, 61)
};



void setupFarbsensor() {

  // Setup and switch off the sensor light
  pinMode(SENSOR_LIGHT_PIN, OUTPUT);
  digitalWrite(SENSOR_LIGHT_PIN, LOW);
  
  // Start TCS34725 color sensor
  if (!_colorSensor.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }


  // Set the RGB pins to output
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Gamma table code from: https://learn.adafruit.com/chameleon-scarf/code
  // Corrects color output based on human perception
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    _gammaTable[i] = x;
  }

}

/**
 * Sets the RGB LED color
 */
void setRgbLedColor(int red, int green, int blue)
{
  int gammaRed, gammaGreen, gammaBlue;
  
  if (USE_GAMMA_CORRECTION == false) {
    // If a common anode LED, invert values
    if (COMMON_ANODE == true) {
      red = MAX_COLOR_VALUE - red;
      green = MAX_COLOR_VALUE - green;
      blue = MAX_COLOR_VALUE - blue;
    }
    analogWrite(RGB_RED_PIN, red);
    analogWrite(RGB_GREEN_PIN, green);
    analogWrite(RGB_BLUE_PIN, blue);
  } else {

    if (COMMON_ANODE == true) {
      gammaRed = MAX_COLOR_VALUE - _gammaTable[red];
      gammaGreen = MAX_COLOR_VALUE - _gammaTable[green];
      gammaBlue = MAX_COLOR_VALUE - _gammaTable[blue];
    } else {
      gammaRed = _gammaTable[red];
      gammaGreen = _gammaTable[green];
      gammaBlue = _gammaTable[blue];
    }
    
    analogWrite(RGB_RED_PIN, gammaRed);
    analogWrite(RGB_GREEN_PIN, gammaGreen);
    analogWrite(RGB_BLUE_PIN, gammaBlue);
  }

  Serial.print("RAW\tRed:"); Serial.print(red);
  Serial.print(", Green:"); Serial.print(green);
  Serial.print(", Blue:"); Serial.print(blue);
  Serial.println();

  Serial.print("GAMMA\tRed:"); Serial.print(gammaRed);
  Serial.print(", Green:"); Serial.print(gammaGreen);
  Serial.print(", Blue:"); Serial.print(gammaBlue);
  Serial.println();
}

int leseFarbsensor() {
  
  float sensedRed, sensedGreen, sensedBlue = 0;
  float accuRed, accuGreen, accuBlue;
  accuRed = 0;
  accuGreen = 0;
  accuBlue = 0;

  static float overallRed = 0;
  static float overallGreen = 0;
  static float overallBlue = 0;
  static int numberOfSamples = 0;

#define NUMS_SAMPPLES 2

  digitalWrite(SENSOR_LIGHT_PIN, HIGH);    
  for (int i=0; i<NUMS_SAMPPLES; i++) {
    // Read from color sensor
    
    _colorSensor.setInterrupt(false);      // turn on LED
    delay(60);                    // takes 50ms to read
    _colorSensor.setInterrupt(true);       // turn off LED
    _colorSensor.getRGB(&sensedRed, &sensedGreen, &sensedBlue);
        
    delay(60);
    accuRed += sensedRed;
    accuGreen += sensedGreen;
    accuBlue += sensedBlue;
  }
  //digitalWrite(SENSOR_LIGHT_PIN, LOW);

  int rawRed = (int)(accuRed/NUMS_SAMPPLES);
  int rawGreen = (int)(accuGreen/NUMS_SAMPPLES);
  int rawBlue = (int)(accuBlue/NUMS_SAMPPLES);

  overallRed += rawRed;
  overallGreen += rawGreen;
  overallBlue += rawBlue;
  numberOfSamples++;

  Serial.print("overall: \t"); Serial.print((int)(overallRed/numberOfSamples));
  Serial.print("\t"); Serial.print((int)(overallGreen/numberOfSamples));
  Serial.print("\t:"); Serial.println((int)(overallBlue/numberOfSamples));

  
  setRgbLedColor(rawRed, rawGreen, rawBlue);

  // Get the name of closest color and print to serial
  ColorName *closestColorName = ColorName::getClosestColorName(_colorNames, 
    NUM_COLOR_NAMES, rawRed, rawGreen, rawBlue);
  Serial.println(closestColorName->getName());

  return closestColorName->getId();
}
