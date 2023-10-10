#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//wifi credentials
const char* ssid = "My phone";
const char* password = "123456789o";
//MQTT cloud url and current running port
const char* mqttServer = "f5b07f1fc11b4a6ea6905a9e1dd894f7.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const int greenPin = 5;
const int bluePin = 4;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT Broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds");
      delay(5000);
    }
  }

  // subscribe to the MQTT topic
  client.subscribe("Receive");
}

void handleMessage(char* topic, byte* payload, unsigned int length) {
  //condition to act upon
  if (strncmp((char*)payload, "1", length) == 0) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    Serial.println("Received Signal: 1");
    Serial.println("Available");
  } else if (strncmp((char*)payload, "0", length) == 0) {
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    Serial.println("Received Signal: 0");
    Serial.println("Not Available");
  }
}

void loop() {
  if (!client.connected()) {
    Serial.println("Reconnecting to MQTT Broker");
    if (client.connect("ESP8266Client")) {
      Serial.println("Reconnected to MQTT Broker");
      client.subscribe("Receive");
    } else {
      Serial.print("Failed to reconnect, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}
