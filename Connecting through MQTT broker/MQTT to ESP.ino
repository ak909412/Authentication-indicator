#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"

#define TIME_ZONE -5

WiFiClientSecure net;
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

// Define GPIO pins
const int pin5 = 5;  
const int pin12 = 12; 

void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1510592825) 
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  
  if (payload[0] == '1') {
    digitalWrite(pin5, HIGH);
    digitalWrite(pin12, LOW);
    Serial.println("green light");
    delay(100);
    digitalWrite(pin5, LOW);
  } else if (payload[0] == '0') {
    digitalWrite(pin5, LOW);
    digitalWrite(pin12, HIGH);
    Serial.println("red light");
    delay(100);
    digitalWrite(pin12, LOW);
  } else {
    digitalWrite(pin5, LOW);
    digitalWrite(pin12, LOW);
    Serial.println("Invalid message received");
  }
}

void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);

  Serial.println("Connecting to AWS IoT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void setup()
{
  Serial.begin(115200);
  
  pinMode(pin5, OUTPUT);
  pinMode(pin12, OUTPUT);
  
  connectAWS();
}

void loop()
{
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
  }
}
