#include "FSManager.h"
#include "sensores.h"
#include "servicios.h"
#include "registro.h"
#include <Wire.h>
#include <SFE_MicroOLED.h> 
#define PIN_RESET 255
#define DC_JUMPER 0

MicroOLED oled(PIN_RESET, DC_JUMPER);
char grau=248;
extern ESP8266WebServer server;
float T, RH, Tacc=0, RHacc=0;
int CO2, P, CO2acc=0, Pacc=0;
int Nmes=0;
unsigned long ahora;
unsigned long siguiente;
unsigned long intervalo = 300000;

void ajaxT() {
     String txt = String(T);
     server.send(200, "text/plain", txt);
     txt = String();
}

void ajaxH() {
     String txt = String(RH);
     server.send(200, "text/plain", txt);
     txt = String();
}

void ajaxC() {
     String txt = String(CO2);
     server.send(200, "text/plain", txt);
     txt = String();
}

void ajaxP() {
     String txt = String(P);
     server.send(200, "text/plain", txt);
     txt = String();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(115200);
  Wire.begin();
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.
  initSensors();
  SPIFFS.begin();
  initHelper(); 
  server.on("/getT", ajaxT);
  server.on("/getH", ajaxH);
  server.on("/getC", ajaxC);
  server.on("/getP", ajaxP);
  server.on("/hoy.csv", mostra);
  server.on("/historics", printHistorics);
  server.on("/datadel", DataDelete);
  server.on("/graf", grafica);
  server.begin();
  syncTime();
  ahora = millis();
  siguiente = ahora + intervalo; 
  ThingSpeakInit(); 
  MQTTInit(); 
}

void loop() {
  getSensors();
  T=getT();
  RH=getRH();
  CO2=getCO2();
  P=getP();
  Nmes++; Tacc+=T; RHacc+=RH; CO2acc+=CO2; Pacc+=P;
  oled.clear(PAGE); oled.setFontType(0); oled.setCursor(0,0);
  oled.print("T:"); oled.print(T); oled.print(String(grau)); oled.print("C");
  oled.setCursor(0,10); oled.print("HR:"); oled.print(RH); oled.print(" %");
  oled.setCursor(0,20); oled.print("CO2:"); oled.print(CO2); oled.print(" p");
  oled.setCursor(0,30); oled.print("IP:"); oled.print(WiFi.localIP());
  oled.display();
  if (millis()>siguiente) {
    digitalWrite(LED_BUILTIN,LOW);  
    siguiente = siguiente + intervalo;
    Tacc=Tacc/Nmes; RHacc=RHacc/Nmes; CO2acc=CO2acc/Nmes; Pacc=Pacc/Nmes;
    guarda(Tacc, RHacc, CO2acc, Pacc);
    ThingSpeakSend(Tacc, RHacc, CO2acc, Pacc);
    MQTTSend(Tacc, RHacc, CO2acc, Pacc);
    Nmes=0; Tacc=0; RHacc=0; CO2acc=0; Pacc=0;
    digitalWrite(LED_BUILTIN,HIGH);
  }  
  espera(3000);      
}
