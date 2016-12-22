
//#define _DEBUG_

#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define USERNAME "snemo"
#define DEVICE_ID "heating_stove"
#define DEVICE_CREDENTIAL "heating_stove"

OneWire oneWire(D7);
OneWire oneWire2(D6);
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);

#define SSID ""
#define SSID_PASSWORD ""

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  sensors2.begin();

  thing.add_wifi(SSID, SSID_PASSWORD);

  thing["stove_temp"] >> [](pson& out){
      out["temp_1"] = sensors.getTempCByIndex(0);
      out["temp_2"] = sensors2.getTempCByIndex(0);
  };
}

void loop() {
  sensors.requestTemperatures();
  sensors2.requestTemperatures();
  thing.handle();
}
