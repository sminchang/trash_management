#ifndef AirQuality_H
#define AirQuality_H

class AirQuality {
private:
  int AO;
  int FAN;

public:
  AirQuality(int ao, int fan);
  void begin();
  int update();
};

#endif
