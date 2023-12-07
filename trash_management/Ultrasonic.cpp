#include "Ultrasonic.h"
#include "Arduino.h"

Ultrasonic::Ultrasonic(int trig, int echo, int red, int green, int blue) {
  TRIG = trig;
  ECHO = echo;
  RED = red;
  GREEN = green;
  BLUE = blue;
  smoothedDistance = 0;
  deadZoneLowerLimit = 10;
  deadZoneUpperLimit = 200;
}

void Ultrasonic::begin() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

int Ultrasonic::update() {
  digitalWrite(TRIG, LOW);
  digitalWrite(ECHO, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long duration = pulseIn(ECHO, HIGH);

  // 초음파를 발산한 후 다시 돌아온 시간을 가지고 거리 측정
  float distance = ((float)(340 * duration) / 10000) / 2;
  
  // 거리값을 평활화하여 노이즈를 줄입니다. 최신 거리 값을 이전 값과 조합하여 노이즈를 감소.
  // smoothingFactor값이 낮을수록 측정된 거리가 더 부드럽게 변화된다.
  float smoothingFactor = 0.3; // 가중치 값, 0에서 1사이의 범위에 있어야함. 값이 1에 가까울수록 이동평균 영향을 강하게 받음.
  smoothedDistance = (distance * smoothingFactor) + (smoothedDistance * (1.0 - smoothingFactor));

  // 데드존 처리
  if (smoothedDistance > deadZoneUpperLimit) {
    smoothedDistance = deadZoneUpperLimit; // 데드존 상한값으로 설정
  } else if (smoothedDistance < deadZoneLowerLimit) {
    smoothedDistance = deadZoneLowerLimit; // 데드존 하한값으로 설정
  }

  // 데드존 범위 내에서는 LED를 제어
  if (smoothedDistance > 2 && smoothedDistance <= 200) {
    if (smoothedDistance > 40) {
      analogWrite(RED, 0);
      analogWrite(GREEN, 255);
      analogWrite(BLUE, 0);
      //delay(1000);
    } else if (smoothedDistance >= 15 && smoothedDistance <= 40) {
      analogWrite(RED, 255);
      analogWrite(GREEN, 255);
      analogWrite(BLUE, 0);
      //delay(1000);
    } else if (smoothedDistance < 15) {
      analogWrite(RED, 255);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      //delay(1000);
    }
  } else {
    // 데드존 범위를 벗어나면 LED를 모두 끔
    analogWrite(RED, 0);
    analogWrite(BLUE, 0);
    analogWrite(GREEN, 0);
    //delay(1000);
  }

  return smoothedDistance;
}
