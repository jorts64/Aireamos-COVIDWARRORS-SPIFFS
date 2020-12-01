#include <WEMOS_SHT3X.h>
SHT3X sht30(0x45);
#include <Adafruit_SGP30.h>
Adafruit_SGP30 sgp30;
#include <Wire.h>  
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#define DBG_OUTPUT_PORT Serial

void initSensors() {
  Wire.begin();
  sgp30.begin();  
  bmp.begin();
}

void getSensors() {
  sht30.get();
  sgp30.IAQmeasure();
}

float getT() {
  return(sht30.cTemp);
}

float getRH() {
  return(sht30.humidity);
}

int getCO2() {
  return(sgp30.eCO2);
}

int getP() {
  return(bmp.readPressure()/100);
}

void syncTime() {
  // TZ offset +1h
  configTime(3600, 0 , "pool.ntp.org", "time.nis.gov");
  // Wait till time is synced
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Syncing time");
  int i = 0;
  while (time(nullptr) < 1000000000ul && i<100) {
    DBG_OUTPUT_PORT.print(".");
    delay(100);
    i++;
  }
  DBG_OUTPUT_PORT.println();
  time_t tnow = time(nullptr);
  struct tm *timeinfo;
  char buffer [80];
  timeinfo = localtime (&tnow);
  strftime (buffer,80,"Local time: %H:%M:%S - %d-%m-%Y.",timeinfo);
  DBG_OUTPUT_PORT.println(buffer);
  timeinfo = gmtime (&tnow);
  strftime (buffer,80,"UTC time: %H:%M:%S - %d-%m-%Y.",timeinfo);
  DBG_OUTPUT_PORT.println(buffer); 
}

void debugTime() {
  DBG_OUTPUT_PORT.println();
  time_t tnow = time(nullptr);
  struct tm *timeinfo;
  char buffer [80];
  timeinfo = localtime (&tnow);
  strftime (buffer,80,"Local time: %H:%M:%S - %d-%m-%Y.",timeinfo);
  DBG_OUTPUT_PORT.println(buffer);
}
