/*
  Modul Wi-Fi for ESP8266
  part of Arduino Mega Server project
*/
String* credentials = handleWiFi();
//
char* ssid = (char*)credentials[0].c_str();
char* pass = (char*)credentials[1].c_str();

//54111004
//
IPAddress ip = SELF_IP;
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);

void wifiInit() {
  initStart_("Wi-Fi");
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  initDone_();
}
