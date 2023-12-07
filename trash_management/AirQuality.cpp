//DO핀은 사용하지 않는다.

#include "AirQuality.h"
#include "Arduino.h"

AirQuality::AirQuality(int ao, int fan) {
  AO = ao;
  FAN = fan;
}

void AirQuality::begin() {
  pinMode(AO, INPUT);
  pinMode(FAN, OUTPUT);
}

int AirQuality::update() {
    if(analogRead(AO) >= 70) {   // 공기질이 70 이상이면
      digitalWrite(FAN, HIGH);  // 쿨링팬 동작
    }
    else {
      digitalWrite(FAN, LOW);
    }
  //}
  return analogRead(AO);
}
