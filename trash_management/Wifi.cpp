/*RX, TX를 매개변수로 받아오면 HTML 파일 로딩이 안되서 모듈 내부에서 지정했다.*/

/*통신 모듈간 동기화 로직을 넣지 않아 동시에 실행시키면 동시성 문제가 발생한다.
특히 입력값을 동시에 바꿔주면 에러가 난다. 근본적인 문제를 해결하지는 못했으나
updateInterval 설정을 통해 아두이노마다 업데이트 주기를 다르게 설정해주면
동시에 입력값을 바꾸는 경우를 제외하고 잘 동작한다...*/

#include "Wifi.h"
#include "WiFiEsp.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX (맨끝핀이 3번)

Wifi::Wifi() {}

char ssid[] = "ian iPhone";
char pass[] = "11223344";
int status = WL_IDLE_STATUS;
WiFiEspServer server(80);
unsigned long updateInterval = 1550; //업데이트 주기, 쓰레기통마다 다르게 설정해서 데드락 문제를 피해야한다.
unsigned long lastUpdateTime;

void Wifi::begin() {
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  
  if(WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  server.begin();
}

void Wifi::update(int ammonia, int capacity) {
    // listen for incoming clients
  WiFiEspClient client = server.available();
   if(client && (millis() - lastUpdateTime > updateInterval)) {
    lastUpdateTime = millis();
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if(c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Connection: close\r\n"  
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
            "\r\n");

          client.print("<!DOCTYPE HTML>\r\n"
          "<html>\r\n"
          "<h1>일립관 B01호</h1>\r\n" //쓰레기통 위치마다 다르게 설정하기
          "<style>\r\n"
          ".n1 { color: red; }\r\n"
          ".n2 { color: green; }\r\n"
          ".n3 { color: black; }\r\n"
          "</style>\r\n");

          //암모니아가 일정 농도 이상되면 해당 문구를 빨간색으로 출력해주는 로직
          if(ammonia >= 70) {
            client.print("<div class='n1'>암모니아 농도: ");
          } else{
            client.print("<div class='n3'>암모니아 농도: ");
          }
          client.print(ammonia);
          client.print("<br></div>\r\n");

          //잔여 용량이 기준치 이상이면 해당 문구를 초록색으로 출력해주는 로직
          if (capacity < 15) {
            client.print("<div class='n2'>잔여 용량: ");
            
          } else{
            client.print("<div class='n3'>잔여 용량: ");
          }
          client.print(capacity);
          client.print("<br></div>\r\n"
          "</html>\r\n");
          break;
        }
        if(c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if(c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}

void Wifi::printWifiStatus() {
    // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
