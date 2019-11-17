#include <ESP8266WiFi.h>       
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
 
void setup() {

 
  Serial.begin(115200);

 
  WiFi.printDiag(Serial);
  Serial.println("Opening configuration portal");
 
  WiFiManager wifiManager;  

  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(60);
 
  if (!wifiManager.startConfigPortal("ESP8266","password"))
  {
     Serial.println("Not connected to WiFi but continuing anyway.");
  } 
  else 
  {
     Serial.println("connected...)");
  }
 
  int connRes = WiFi.waitForConnectResult();

  if (WiFi.status()!=WL_CONNECTED)
  {
      Serial.println("failed to connect, finishing setup anyway");
  } 
  else
  {
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
  }
}
  
void loop() {
 delay(10000); 
}
