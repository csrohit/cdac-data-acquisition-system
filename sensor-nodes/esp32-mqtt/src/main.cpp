#include <Arduino.h>

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
uint8_t state = 0;

typedef struct struct_mqtt_frame{
    uint8_t peripheral_addr;
    uint8_t cmd;
}mqtt_frame_t;


#define LED 23
void setup_wifi();
void callback(char *topic, byte *message, unsigned int length);
// Replace the next variables with your SSID/Password combination
const char *ssid = "nimkar";
const char *password = "Flanker-H";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.0.138";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  uint8_t state = 1;

  digitalWrite(LED, state);

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  // status = bme.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" length: ");
  mqtt_frame_t * frame = (mqtt_frame_t *)message;
  Serial.printf("Device id: %#x, Cmd: %#x\n", frame->peripheral_addr, frame->cmd);

  switch (frame->cmd)
  {
  case 0x01:
    state = 1;
    digitalWrite(LED, state);
    break;
  case 0x02:
    state = 0;
    digitalWrite(LED, state);
    break;
  case 0x03:
    state = !state;
    digitalWrite(LED, state);
    break;
  case 0x04:
    state = !1;
    digitalWrite(LED, state);
    delay(500);
    state = !1;
    digitalWrite(LED, state);
    break;

  default:
    break;
  }
  // String messageTemp;

  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)message[i]);
  //   messageTemp += (char)message[i];
  // }
  Serial.println(length);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("purv_esp32");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
