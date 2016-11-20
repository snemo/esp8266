#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"


const char *ssid =  "666";   // cannot be longer than 32 characters!
const char *pass =  "****";   //

const char *mqtt_server = "m21.cloudmqtt.com";
const int mqtt_port = 18266;
const char *mqtt_user = "zgchmnsv";
const char *mqtt_pass = "*****";
const char *mqtt_client_name = "XXXX"; // Client connections cant have the same connection name

const int dhtPin = 2;
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE, 12); // The 12 might need tuning so play with 10-15 till it works for the ESP8266
unsigned long previousMillis = 0;
const long interval = 10000;   


WiFiClient wclient;
PubSubClient client(mqtt_server, mqtt_port, wclient);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  dht.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect("myEsp8266-w", mqtt_user, mqtt_pass)){
        Serial.println("Connected to MQTT server");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }

  SendTempHumid();
}

// Non-Blocking delay
void SendTempHumid(){
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      //return; This will ensure that data is always sent
    }

    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C ");

    String sStr = String(h)+"% - "+String(t)+"C";
    char testChar[sStr.length()];
    sStr.toCharArray(testChar, sStr.length());
    
    client.publish("Humidity", testChar );
  }
}
