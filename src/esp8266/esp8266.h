#include <Arduino.h>
#include <Wire.h>
#include <ezTime.h>
#include <DHTesp.h>
#include <ESP8266WiFiMulti.h>

#include "../common/communication.h"

#define DHT_PIN 14
#define ALARM_SWITCH_PIN 12
#define SNOOZE_BUTTON_PIN 13

ESP8266WiFiMulti wifiMulti;
Timezone timeZone;
DHTesp dht;
PacketMVStatus status;

unsigned long lastDHTRead;
unsigned long lastNTPRead;

void setup()
{
  pinMode(ALARM_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SNOOZE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(112500);
  delay(1500);

  Wire.begin();

#include "WiFiAPs.h"
  Serial.println("Connecting to Wifi");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.write('.');
    delay(100);
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address:     "));
  Serial.println(WiFi.localIP());

  // Uncomment the line below to see what it does behind the scenes
  // setDebug(INFO);

  // NTP SETUP
  waitForSync();

  timeZone.setLocation(F("America/Chicago"));

  dht.setup(DHT_PIN, DHTesp::DHT22);
}

void loop()
{
  digitalWrite(LED_BUILTIN, LOW);
  if (millis() - lastDHTRead > 5000)
  {
    Serial.println(F("Reading DHT22"));
    lastDHTRead = millis();
    TempAndHumidity TaH = dht.getTempAndHumidity();
    status.tempF = DHTesp::toFahrenheit(TaH.temperature);
    status.humidity = TaH.humidity;
  }

  if (millis() - lastNTPRead > 900000) // every 15 minutes
  {
    Serial.println(F("Getting Time through NTP"));
    lastNTPRead = millis();
    updateNTP();
  }
  events();
  time_t time = timeZone.now();
  status.hour = hour(time);
  status.minute = minute(time);
  status.day = day(time);
  status.month = month(time);
  status.year = year(time);

  status.isAlarmOn = digitalRead(ALARM_SWITCH_PIN);

  Serial.println(F("Sending Packet"));
  Wire.beginTransmission(0x08);
  uint8_t *bytes = (uint8_t *)&status;
  for (uint8 i = 0; i < sizeof(PacketMVStatus); i++)
  {
    Wire.write(bytes[i]);
  }
  Wire.endTransmission();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}
