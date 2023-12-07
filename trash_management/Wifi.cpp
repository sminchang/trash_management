/*RX, TX를 매개변수로 받아오면 HTML 파일을 못불러와서 모듈 내부에서 지정했다.*/
/*통신 모듈간 동기화 로직을 넣지 않아 동시에 실행시키면 동시성 문제가 발생한다.
아두이노를 하나만 실행시키고 연결된 후 3초정도 지나서
다음 아두이노를 실행시켜야 문제가 안생긴다.*/

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
  if(client) {
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

          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<h1>일립관 B01호</h1>\r\n");
          client.print("<style>\r\n");
          client.print(".n1 { color: red; }\r\n");
          client.print(".n2 { color: green; }\r\n");
          client.print(".n3 { color: black; }\r\n");
          client.print("</style>\r\n");

          //암모니아가 일정 농도 이상되면 해당 문구를 빨간색으로 출력해주는 로직
          if(ammonia >= 70) {
            client.print("<div class='n1'>암모니아 농도: ");
            client.print(ammonia);
            client.print("<br></div>\r\n");
          } else{
            client.print("<div class='n3'>암모니아 농도: ");
            client.print(ammonia);
            client.print("<br></div>\r\n");
          }

          //잔여 용량이 기준치 이상이면 해당 문구를 초록색으로 출력해주는 로직
          if (capacity < 30) {
            client.print("<div class='n2'>잔여 용량: ");
            client.print(capacity);
            client.print("<br></div>\r\n");
          } else{
            client.print("<div class='n3'>잔여 용량: ");
            client.print(capacity);
            client.print("<br></div>\r\n");
          }
          client.print("</html>\r\n");
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