#include "MHZ19.h"          // Uses https://github.com/strange-v/MHZ19, don't use another with same name                                     
MHZ19 mhz(&Serial);

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme; 

void initSensors() {
  Serial.swap();
  bme.begin(0x76);
}

void getSensors() { // Not needed for these sensors 
}

float getT() {
  return(bme.readTemperature());
}

float getRH() {
  return(bme.readHumidity());
}

int getCO2() {
  MHZ19_RESULT response = mhz.retrieveData();
  if (response == MHZ19_RESULT_OK){  
    return(mhz.getCO2());
  }
  else {
    return(4000);
  }
}

int getP() {
  return(bme.readPressure()/100);
}

void syncTime() {
  configTime(3600, 0 , "pool.ntp.org", "time.nis.gov");   // TZ offset +1h
  int i = 0;
  while (time(nullptr) < 1000000000ul && i<100) {
    delay(100);
    i++;
  }
  time_t tnow = time(nullptr);
}
