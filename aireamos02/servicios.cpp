#include "config.h"
#include <ESP8266WiFi.h>

#include <PubSubClient.h>
WiFiClient MqttClient;
PubSubClient client(MqttClient);

#include "ThingSpeak.h"
unsigned long myChannelNumber = THINGSPEAKCHANNEL;
const char * myWriteAPIKey = THINGSPEAKKEY;
WiFiClient  TSclient;

#define DBG_OUTPUT_PORT Serial

void ThingSpeakInit(){
#if defined THINGSPEAKACTIVE  
  ThingSpeak.begin(TSclient);
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

void MQTTInit(){
#if defined MQTT_ACTIVE  
    client.setServer(MQTT_SERVER, MQTT_PORT);
#endif   
}


void MQTTSend(float T, float RH, int CO2, int P){
#if defined MQTT_ACTIVE  
  if (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);    
#if defined MQTT_USER    
    client.connect(clientId.c_str(),MQTT_USER,MQTT_PASS);
#else
    client.connect(clientId.c_str());
#endif   
  }
  static char buffMQTT[7];
  dtostrf(T, 6, 2, buffMQTT);
  client.publish(MQTT_TOPIC_T, buffMQTT);
  dtostrf(RH, 6, 2, buffMQTT);
  client.publish(MQTT_TOPIC_RH, buffMQTT);
  dtostrf(CO2, 6, 2, buffMQTT);
  client.publish(MQTT_TOPIC_CO2, buffMQTT);
  dtostrf(P, 6, 2, buffMQTT);
  client.publish(MQTT_TOPIC_P, buffMQTT);
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.println("Enviado MQTT");
  DBG_OUTPUT_PORT.println("");
#endif     
}
