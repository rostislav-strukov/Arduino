#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

const char* ssid = "TP-LINK_D7AE80";
const char* password = "audis430";
int ledPin = 16;
int analogPin = A0;
int inPin = 4;
int val = 0;

void setup () {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(inPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
}

void loop() {
   val = analogRead(analogPin);   // read the input pin
    digitalWrite(analogPin, val);
    Serial.println(val);
    
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;

    time_t now = time(nullptr);

    const int capacity = JSON_ARRAY_SIZE(2) * JSON_OBJECT_SIZE(4);
    StaticJsonDocument<capacity> doc;

    JsonObject root = doc.to<JsonObject>();
    root["id"] = 1;
    root["timeStamp"] = now;
    
    JsonObject deviceDataList = root.createNestedObject("deviceDataList");
    deviceDataList["id"] = "1";
    deviceDataList["typeId"] = 3;
    deviceDataList["timeStamp"] = now;
    deviceDataList["value"] = analogPin;

    client.println("POST /api/v2/" 
    "/groups/arduinojson/data HTTP/1.1");
    client.connect("http://huspi.com:6081/Device/PostFarmData", 80);
    client.println("Host: ihuspi.com");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(measureJson(doc));
    client.println("Content-Type: application/json");
//    client.println("X-AIO-Key: " IO_KEY);
    
//    client.println();
    serializeJson(doc, client);








    
//    client.addHeader("Content-Type", "application/x-www-form-urlencoded");
//    int httpCode = client.POST(postData);
//    String payload = client.getString();
// 
//    Serial.println(httpCode);
//    Serial.println(payload);
 
//    client.end();
 
  }
 
  delay(30000);
 
}
