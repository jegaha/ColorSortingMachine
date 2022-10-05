
#include "display.h"
#include "transport.h"
#include "rampe.h"
#include "farbsensor.h"

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupTransport();
  setupRampe();
  setupFarbsensor();
}

void loop() {
  static int farben[2] = { 0 }; 
  static int zaehler[6] = { 0 };

  dreheScheibe();

  zaehler[farben[0]] ++;
  displayCounter(zaehler);
  
  stelleRampeAufPosition(farben[0]);

  farben[0] = farben [1];
  farben[1] = leseFarbsensor();
  
}
