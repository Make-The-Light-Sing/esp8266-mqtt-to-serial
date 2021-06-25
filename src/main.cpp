#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config-env.h"

void setup_wifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, callback, espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  Serial.println("Setup completed...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(100);
}

void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connection established ");
  Serial.print("=> IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Serial.println("Data received");
  // Serial.print("Topic: ");
  // Serial.println(topic);
  // Serial.print("Payload: ");
  for(unsigned int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  //Serial.println((char) payload[0]);
  Serial.println();
}

void reconnect() {
  //Boucle jusqu'à obtenur une reconnexion
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Server...");
    if (client.connect(MQTT_CLIENT_NAME)) {
      Serial.println("OK");
      client.subscribe(MQTT_SUBSCRIBE_TOPIC);
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      delay(5000);
    }
  }
}