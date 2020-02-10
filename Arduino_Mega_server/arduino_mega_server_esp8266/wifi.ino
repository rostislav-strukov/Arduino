/*
  Modul Wi-Fi for ESP8266
  part of Arduino Mega Server project
*/

int ledPin = 16;
int analogPin = A0;
int inPin = 4;
int val = 0;



//54111004
//
IPAddress ip = SELF_IP;
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);

void wifiInit() {

  String* credentials = handleWiFi();
  Serial.println((char*)credentials[0].c_str());
  Serial.println((char*)credentials[1].c_str());
//
  char* ssid = (char*)credentials[0].c_str();
  char* pass = (char*)credentials[1].c_str();

  boolean result = WiFi.softAP("ESP8266");
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }
  
  initStart_("Wi-Fi");
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);

  const String farmId = getMacAddress();
   
   val = analogRead(analogPin);   // read the input pin
   
   digitalWrite(analogPin, val);
   Serial.print("MAC: ");
   Serial.println(farmId);
   Serial.print("Sensor: ");
   Serial.println(val);
   //server.handleClient();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
      WiFiClient client;
      
      time_t now = time(nullptr);   
      const int capacity = JSON_ARRAY_SIZE(2) * JSON_OBJECT_SIZE(4);
      StaticJsonDocument<capacity> doc;

      JsonObject root = doc.to<JsonObject>();
      root["id"] = farmId;
      root["timeStamp"] = now;
      root["username"] = "Vivaldi";
    
      JsonObject deviceDataList = root.createNestedObject("deviceDataList");
      deviceDataList["id"] = "1";
      deviceDataList["typeId"] = 3;
      deviceDataList["timeStamp"] = now;
      deviceDataList["value"] = analogPin;

      Serial.println(WiFi.localIP());
      Serial.println(WiFi.hostname());
    
      client.println("POST /api/v2/" 
        "/groups/arduinojson/data HTTP/1.1");
      client.connect("http://huspi.com:6081/Farms/PostFarmData", 80);
      client.println("Host: ihuspi.com");
      client.println("Connection: close");
      client.print("Content-Length: ");
      client.println(measureJson(doc));
      client.println("Content-Type: application/json");
      //client.println("X-AIO-Key: " IO_KEY);
    
      client.println();
      serializeJson(doc, client);

      delay(2000);
      
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  initDone_();
}

String getMacAddress() {
  byte mac[6];
  
  WiFi.macAddress(mac);
  String cMac = "";
  
  for (int i = 0; i < 6; ++i) {
    cMac += String(mac[i],HEX);
    
    if(i<5)
      cMac += "-";
  }
  cMac.toUpperCase();
  
  return cMac;
}
