#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <vector>

SoftwareSerial arduinoSerial(D1, D2); // RX sur D2, TX sur D3

const char* ssid = "XXX";
const char* password = "XXX";

const char* mqtt_server = "192.168.233.7";
const char* mqtt_user = "admin";
const char* mqtt_password = "admin";

WiFiClient WFClient;
PubSubClient client(WFClient);

void setup_wifi() {
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("> Connected to WiFi !");
}

void setup() {
  Serial.begin(115200); // Debug
  arduinoSerial.begin(4800);  // Communication avec Arduino
  
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
}

void loop() {
  while(!client.connected()) {
    Serial.print("> Connecting to MQTT...");
    if(client.connect("ArduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("> Connected to MQTT");
      client.publish("iot/status", "Hello mqtt");
    }
    else {
      Serial.print("> Error on the connection... RC=");
      Serial.print(client.state());
      Serial.println(" New try in 5 seconds");
      delay(5000);
    }
  }
  
  client.loop();
  
  static unsigned long lastMsg = 0;
  if ((millis() - lastMsg) > 5000) {
    lastMsg = millis();
    client.publish("iot/status", "I'm up !");
    Serial.println("Message sent.");

    // Lire les données de l'Arduino proprement
    if (arduinoSerial.available()) {
      String data = arduinoSerial.readStringUntil('\n'); // Lire jusqu'à un retour à la ligne
      data.trim(); // Nettoyer les espaces vides et caractères parasites
      
      if (data.length() > 0) {
        data = data.substring(64);
        Serial.println("Données reçues : " + data);
        client.publish("iot/sensors", data.c_str());
      }
    }
  }
}
