#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <time.h>
#include <DNSServer.h>
#include <WiFiManager.h>

const char* ssid = "Greenvice"; 
const char* password = "password";
int ledPin = 16;
int analogPin = A0;
int inPin = 4;
int val = 0;


ESP8266WebServer server(80);

void setup () {
 
  Serial.begin(115200);
  WiFi.printDiag(Serial);
  Serial.println("Opening configuration portal");

  WiFiManager wifiManager;

  if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(60);
  
  if (!wifiManager.startConfigPortal(ssid, password))
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
  
  pinMode(inPin, INPUT);
  pinMode(ledPin, OUTPUT);
  WiFi.hostname("greenhost");
}

void loop() {

   const String farmId = WiFi.macAddress();
   
   val = analogRead(analogPin);   // read the input pin
   
   digitalWrite(analogPin, val);
   Serial.print("MAC: ");
   Serial.println(farmId);
   Serial.print("Sensor: ");
   Serial.println(val);
   server.handleClient();
   
   if (WiFi.status() == WL_CONNECTED) {

      WiFiClient client;
    
      time_t now = time(nullptr);   
      const int capacity = JSON_ARRAY_SIZE(2) * JSON_OBJECT_SIZE(4);
      StaticJsonDocument<capacity> doc;

      JsonObject root = doc.to<JsonObject>();
      root["id"] = farmId;
      root["timeStamp"] = now;
    
      JsonObject deviceDataList = root.createNestedObject("deviceDataList");
      deviceDataList["id"] = "1";
      deviceDataList["typeId"] = 3;
      deviceDataList["timeStamp"] = now;
      deviceDataList["value"] = analogPin;

      Serial.println(WiFi.localIP());
      Serial.println(WiFi.hostname());
    
      client.println("POST /api/v2/" 
        "/groups/arduinojson/data HTTP/1.1");
      client.connect("http://huspi.com:6081/Device/PostFarmData", 80);
      client.println("Host: ihuspi.com");
      client.println("Connection: close");
      client.print("Content-Length: ");
      client.println(measureJson(doc));
      client.println("Content-Type: application/json");
      //client.println("X-AIO-Key: " IO_KEY);
    
      client.println();
      serializeJson(doc, client);

      delay(4000);
  }
}
