#include "config.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
unsigned long myChannelNumber = THINGSPEAKCHANNEL;
const char * myWriteAPIKey = THINGSPEAKKEY;

WiFiClient  client;
#define DBG_OUTPUT_PORT Serial

void ThingSpeakInit(){
#if defined THINGSPEAKACTIVE  
  ThingSpeak.begin(client);
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.println("Iniciado ThinkSpeak");
  DBG_OUTPUT_PORT.println("");
#endif  
}
 

void ThingSpeakSend(float T, float RH, int CO2, int P){
#if defined THINGSPEAKACTIVE  
  ThingSpeak.setField(1,T);
  ThingSpeak.setField(2,RH);
  ThingSpeak.setField(3,CO2);
  ThingSpeak.setField(4,P);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.println("Enviado a ThinkSpeak");
  DBG_OUTPUT_PORT.println("");
#endif  
  
}


void enviaMQTT(){
  
}
