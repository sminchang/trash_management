#ifndef WIFI_H
#define WIFI_H

class Wifi {
private:


public:
  Wifi();
  void begin();
  void update(int ammonia, int capacity);
  void printWifiStatus();
};

#endif