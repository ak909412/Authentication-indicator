//importing the required libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//Both application and the ESP module should be connected to the same wifi network
const char* ssid = "Veris"; //wWifi SSID
const char* password = "veris@1234"; //Wifi Password

const int greenPin = 5;
const int bluePin = 4;

// the active port to receive command on
ESP8266WebServer server(80);


void setup() {

//setting the output pins on which the LED strips will bw connected
// initially keeping the strips off
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);
  
// setting the Baud rate
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

//printing the IP address of ESP8266

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

// controlling the LED strips according to the command received

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
  }
    server.send(200, "text/plain", "Relay toggled for 1 second");
  } else {
    server.send(400, "text/plain", "Invalid action");
  }
}
