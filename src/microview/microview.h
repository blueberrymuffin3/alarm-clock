#include <Arduino.h>
#include <MicroView.h>
#include <Wire.h>
#include "../common/communication.h"
#include "img/img.h"

PacketMVStatus status;

#define ALARM_BYTE_POS 118
inline void drawAlarm(bool isOn)
{
  uint8_t *bitmap = isOn ? alarmY : alarmN;
  screenmemory[ALARM_BYTE_POS + 0] = bitmap[0];
  screenmemory[ALARM_BYTE_POS + 1] = bitmap[1];
  screenmemory[ALARM_BYTE_POS + 2] = bitmap[2];
  screenmemory[ALARM_BYTE_POS + 3] = bitmap[3];
  screenmemory[ALARM_BYTE_POS + 4] = bitmap[4];
  screenmemory[ALARM_BYTE_POS + 5] = bitmap[5];
  screenmemory[ALARM_BYTE_POS + 6] = bitmap[6];
  screenmemory[ALARM_BYTE_POS + 7] = bitmap[7];
}

inline void drawTime()
{
  uView.setCursor(-7, 0);
  uView.setFontType(2);

  bool isAM = status.hour < 12;
  uint8_t twelveHour = (status.hour + 11) % 12 + 1;

  if (twelveHour < 10)
    uView.print(" ");
  uView.print(twelveHour);
  uView.print(" ");
  if (status.minute < 10)
    uView.print("0");
  uView.print(status.minute);

  if (millis() / 500 & 1)
  {
    uView.circleFill(20, 4, 2);
    uView.circleFill(20, 12, 2);
  }

  uView.setFontType(0);
  uView.setCursor(52, 0);
  uView.print(isAM ? "AM" : "PM");

  drawAlarm(status.isAlarmOn);
}

void receiveEvent(int len)
{
  uint8_t *bytes = (uint8_t *)&status;
  for (uint8_t i = 0; i < sizeof(PacketMVStatus) && Wire.available(); i++)
  {
    bytes[i] = Wire.read();
  }
  while(Wire.available()) Wire.read();
}

void setup()
{
  uView.begin();
  uView.contrast(0);
  uView.display();
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  while(status.year == 0) delay(10);
}

void loop()
{
  uView.clear(PAGE);
  drawTime();

  uView.setCursor(0, 20);
  uView.print(status.year);
  uView.print('-');
  if(status.month < 10) uView.print('0');
  uView.print(status.month);
  uView.print('-');
  if(status.day < 10) uView.print('0');
  uView.print(status.day);

  uView.setCursor(0, 32);
  uView.print(F(" "));
  uView.print((int)status.tempF);
  uView.print('\xF7');
  uView.print(F("F "));
  uView.print((int)status.humidity);
  uView.print(F("%"));
  // uView.setCursor(0, 40);
  // uView.print(" \xA7 \xF7 ");

  uView.display();

  delay(100);
}
