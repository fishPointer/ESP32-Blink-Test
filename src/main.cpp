#include <Arduino.h>
#include <WiFi.h>
const char *SSID = "Breath of the Wifi";
const char *PWD = "supervolcano55";
const char *HUTAO = "192.168.0.193";
void connectToWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++) {
    Serial.print(char(payload[i]));
  }
  Serial.println();
  Serial.println(length);
}



#include <PubSubClient.h>
// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
//char *mqttServer = HUTAO;
int mqttPort = 1616;
void setupMQTT() {
  mqttClient.setServer(HUTAO, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}



void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupMQTT();
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("corn");
      }
      
  }
}
void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
}