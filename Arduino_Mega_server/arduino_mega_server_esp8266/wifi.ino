/*
  Modul Wi-Fi for ESP8266
  part of Arduino Mega Server project
*/
// char** credentials = handleWiFi();
//
char* ssid;
char* pass;
char** credentials;

//54111004
//
IPAddress ip = SELF_IP;
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);

void wifiInit() {
  
  credentials = handleWiFi();

  
  initStart_("Wi-Fi");

  ssid = (char*)credentials[0];
  pass = (char*)credentials[1];

  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  Serial.println(pass);
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
