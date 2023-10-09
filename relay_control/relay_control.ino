#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Veris";
const char* password = "veris@1234";

const int greenPin = 5;
const int bluePin = 4;


ESP8266WebServer server(80);


void setup() {
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/api/control_relay", HTTP_POST, handleControlRelay);
  server.begin();
}

void loop() {
  server.handleClient();

}



void handleControlRelay() {
  String action = server.arg("plain");

  if (action == "1") {
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    Serial.println("Received Signal: 1");
    Serial.println("Available");
    server.send(200, "text/plain", "Relay turned on");
  } else if (action == "0") {
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    Serial.println("Received Signal: 0");
    Serial.println("Not Available");
    server.send(200, "text/plain", "Relay turned off");
  } else if (action == "2") {
    Serial.println("Received Signal: 2");
    Serial.println("About to be available");
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    delay(4000);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    delay(4000);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    delay(4000);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    delay(4000);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);

    server.send(200, "text/plain", "Relay toggled for 1 second");
  } else {
    server.send(400, "text/plain", "Invalid action");
  }
}
