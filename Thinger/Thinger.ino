
//#define _DEBUG_

#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include "DHT.h"

#define USERNAME "snemo"
#define DEVICE_ID "test"
#define DEVICE_CREDENTIAL "r1Sbgj5qGxNp"


const int dhtPin = 2;
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE, 20); // The 12 might need tuning so play with 10-15 till it works for the ESP8266


#define SSID "Vendavo_Guests"
#define SSID_PASSWORD "Vend@vo1"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);

  thing.add_wifi(SSID, SSID_PASSWORD);

  dht.begin();

  thing["temperature"] >> [](pson& out){
      out["temp"] = dht.readTemperature();
      out["humidity"] = dht.readHumidity();
  };
}

void loop() {
  thing.handle();
}
