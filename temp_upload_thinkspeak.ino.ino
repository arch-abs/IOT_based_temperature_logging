#ifndef __CC3200R1M1RGC__

#include <SPI.h>
#endif
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_TMP006.h"
#include <stdlib.h>
//#define USE_USCI_B1 

char tspkAddr[] = "api.thingspeak.com";
String apikey = "YFU9F79ZWQHPRFMD";
const int updt_inter = 15 * 1000;

char buffer[25];

char ssid[] = "vorteX";
char password[] = "12345678";
WiFiClient client;

unsigned long lastConnectionTime = 0;
boolean lastConnected = false;
const unsigned long postingInterval = 10*1000;
int failedCounter = 0;
Adafruit_TMP006 tmp006(0x41);
void setup() {
  Serial.begin(115200);
  Serial.print("Attempting to connect to network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);    
  }

  if(!tmp006.begin()){
    Serial.println("No sensor found...!!!");
    while(1);  
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.println("\nWaiting for ip address");

  while(WiFi.localIP()==INADDR_NONE){
      Serial.print(".");
      delay(500);    
    }

  Serial.print("\nIP obtained...!!!");
  //printWiFiStatus();
  
  // put your setup code here, to run once:
  }

void loop() {

  while(client.available()){
      char c = client.read();
      Serial.print(c);
    }
    
  if(!client.connected() && lastConnected){
    Serial.println("\ndisconnection...!!!");
    client.stop(); 
  }

  if(!client.connected() && (millis()-lastConnectionTime > postingInterval)){
    float objt = tmp006.readObjTempC();
    float diet = tmp006.readDieTempC();
    String sdiet = dtostrf(diet, 3, 3, buffer);
    Serial.print("Obj temp: ");
    Serial.println(objt);
    Serial.print("Die temp: ");
    Serial.println(sdiet);
    updateThinkSpeak("field1="+sdiet);
  
    }
  // put your main code here, to run repeatedly: 
  lastConnected = client.connected();
}

void updateThinkSpeak(String tsData){
  if(client.connect(tspkAddr, 80)){
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thinkspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apikey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(tsData.length());
      client.print("\n\n");
      client.print(tsData);
  }
}
