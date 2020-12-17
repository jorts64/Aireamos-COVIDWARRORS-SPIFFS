#include "MHZ19.h"                                        
MHZ19 myMHZ19;

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme; 

void initSensors() {
  Serial.swap();
  myMHZ19.begin(Serial);  
  myMHZ19.setRange(2000);  
  myMHZ19.autoCalibration(false);    
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
  return(myMHZ19.getCO2());
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
