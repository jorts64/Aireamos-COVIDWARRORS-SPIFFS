#include "MHZ19.h"                                        
#include <SoftwareSerial.h>  
#define RX_PIN D5                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN D6                                          // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600 

MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN); 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; 

#define DBG_OUTPUT_PORT Serial

void initSensors() {
  mySerial.begin(BAUDRATE); 
  myMHZ19.begin(mySerial);  
  myMHZ19.setRange(2000);  
  myMHZ19.autoCalibration(false);    
  bme.begin(0x76);
}

void getSensors() {
// Not needed for these sensors 
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
