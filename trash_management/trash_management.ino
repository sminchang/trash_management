/*<쿨링팬 연결 시 주의사항>
  정확한 원인은 모르겠으나 다른 모듈의 전원을 연결하면 쿨링팬 동작이 멈출 때가 있다.
  이리저리 잘 끼우다보면 동작한다.
*/
/*<wifi 모듈 연결 시 주의사항>
  접속하는 브라우저가 사용하는 wifi와 esp 모듈을 연결한 wifi가 같아야한다.
  그렇지 않으면 NAT 사용 때문에 IP가 꼬이는 문제가 생기는 것 같다.
*/
/*핫스팟 틀어놓고 핫스팟 연결 UI 띄워놔야 정상 접속된다. 안그러면 버퍼링 걸린다.*/

#include "AirQuality.h"
#include "Ultrasonic.h"
#include "Wifi.h"

//AirQuality
const int ao = A0;     //MQ135 공기질 센서, analogRead
const int fan = 12;    //쿨링팬, digitalWrite

//Ultrasonic
const int trig = 7;   //HC-SR04 초음파 센서, digitalWrite
const int echo = 8;   //HC-SR04 초음파 센서, digitalWrite
const int red = 9;    //RGB, analogWrite
const int green = 10;  //RGB, analogWrite
const int blue = 11;   //RGB, analogWrite

//모니터링 알림
int ammonia;
int capacity;

AirQuality AirQuality(ao, fan);
Ultrasonic Ultrasonic(trig, echo, red, green, blue);
Wifi Wifi;

void setup() {
  Serial.begin(9600);
  AirQuality.begin();
  Ultrasonic.begin();
  Wifi.begin();
}

void loop() {
  ammonia = AirQuality.update();
  capacity = Ultrasonic.update();
  Wifi.update(ammonia, capacity); 
}
