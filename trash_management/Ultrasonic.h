#ifndef Ultrasonic_H
#define Ultrasonic_H

class Ultrasonic {
private:
  int TRIG;
  int ECHO;
  int RED;
  int GREEN;
  int BLUE;
  float smoothedDistance;
  int deadZoneLowerLimit;
  int deadZoneUpperLimit;

public:
  Ultrasonic(int trig, int echo, int red, int green, int blue);
  void begin();
  int update();
};

#endif
