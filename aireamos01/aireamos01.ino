#include "FSManager.h"
#include "sensores.h"
#include "servicios.h"
#include <Wire.h>
#include <SFE_MicroOLED.h> 
#define PIN_RESET 255
#define DC_JUMPER 0

MicroOLED oled(PIN_RESET, DC_JUMPER);
char grau=248;
extern ESP8266WebServer server;
float T, RH;
int CO2,P;
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

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.

  initSensors();
  SPIFFS.begin();
  initHelper(); //inclou connexió Wifi i server
                // Aquí podem afegir altres funcions server.on()
  server.on("/getT", ajaxT);
  server.on("/getH", ajaxH);
  server.on("/getC", ajaxC);
  server.begin();
  syncTime();
  ahora = millis();
  siguiente = ahora + intervalo; 
  ThingSpeakInit(); 
}

void loop() {
  digitalWrite(LED_BUILTIN,LOW);  
  getSensors();
  T=getT();
  RH=getRH();
  CO2=getCO2();
  debugTime();
  DBG_OUTPUT_PORT.print("Temperatura (ºC): ");
  DBG_OUTPUT_PORT.println(T);
  DBG_OUTPUT_PORT.print("Humedad relativa (%): ");
  DBG_OUTPUT_PORT.println(RH);
  DBG_OUTPUT_PORT.print("Concentración CO2 (ppm): ");
  DBG_OUTPUT_PORT.println(CO2);
  DBG_OUTPUT_PORT.println("");
  oled.clear(PAGE);
  oled.setFontType(0);
  oled.setCursor(0,0);
  oled.print("T:");
  oled.print(T);
  oled.print(" ");
  oled.print(grau);
  oled.print("C");
  oled.setCursor(0,10);
  oled.print("HR:");
  oled.print(RH);
  oled.print(" %");
  oled.setCursor(0,20);
  oled.print("CO2:");
  oled.print(CO2);
  oled.print(" p");
  oled.setCursor(0,30);
  oled.print("IP:");
  oled.print(WiFi.localIP());
  oled.display();
  digitalWrite(LED_BUILTIN,HIGH);
  if (millis()>siguiente) {
    siguiente = siguiente + intervalo;
    ThingSpeakSend(T, RH, CO2, P);
  }  
  espera(3000);      
}
