#define ERSTE 20
#define ZWEITE 54
#define DRITTE 83
#define VIERTE 120
#define FUENFTE 160

#define RAMPEN_SERVO_PIN 3
#include <Servo.h>

Servo RampenServo;  // create servo object to control a servo
int winkel[] = { ERSTE, ZWEITE, DRITTE, VIERTE, FUENFTE };

void stelleRampeAufPosition(int pos) {
  if (pos != 0) {
    RampenServo.write(winkel[pos-1]);
  }
}

void setupRampe()
{
  RampenServo.attach(RAMPEN_SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  stelleRampeAufPosition(1);
}
