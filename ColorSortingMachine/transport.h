//#define ACCEL 1

#define TRANSPORT_SCHRITTE 4096
#define TRANSPORT_TEILUNG 10
#define motorPin1  7     // IN1 on ULN2003 ==> Blue   on 28BYJ-48
#define motorPin2  8     // IN2 on ULN2004 ==> Pink   on 28BYJ-48
#define motorPin3  9    // IN3 on ULN2003 ==> Yellow on 28BYJ-48
#define motorPin4  10    // IN4 on ULN2003 ==> Orange on 28BYJ-48

#ifdef ACCEL
  #include <AccelStepper.h>
  #define HALFSTEP 8
  // NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48
  AccelStepper ScheibenMotor(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
#else
  #include <CheapStepper.h>
  CheapStepper ScheibenMotor(motorPin1, motorPin2, motorPin3, motorPin4); //Richtungszuweisung des Motors boolean moveClockwise = true;
  #define RPM 10
#endif

void setupTransport() 
{
  #ifdef ACCEL  
    ScheibenMotor.setMaxSpeed(400.0);
    ScheibenMotor.setSpeed(300);
    ScheibenMotor.setAcceleration(2000);
    ScheibenMotor.setCurrentPosition(0); 
  #else
    ScheibenMotor.setRpm(RPM) ;
  #endif
}

#ifdef ACCEL
void dreheScheibe(){
  static int realPos = 0;
  realPos++;

  if (realPos>10) {
    realPos = 1;
    ScheibenMotor.setCurrentPosition(0);
  }
  
  int moveTo = map (realPos, 0, TRANSPORT_TEILUNG, 0, TRANSPORT_SCHRITTE);
  Serial.println(moveTo);
  ScheibenMotor.moveTo(moveTo);
  
  while (ScheibenMotor.distanceToGo() != 0) {
  ScheibenMotor.run();
  }
}
#else
void dreheScheibe(){
  static int realPos = 0;
  realPos++;

  static int lastMoveTo = 0;
  
  if (realPos>10) {
    realPos = 1;
    lastMoveTo = 0;
  }
  
  int moveTo = map (realPos, 0, TRANSPORT_TEILUNG, 0, TRANSPORT_SCHRITTE);
  Serial.println(moveTo);
  
  ScheibenMotor.move(true, moveTo-lastMoveTo);  
  lastMoveTo = moveTo;
}

#endif
