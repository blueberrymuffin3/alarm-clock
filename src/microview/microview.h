#include <Arduino.h>
#include <MicroView.h>
#include "img/img.h"

MicroViewWidget *widget, *widget2;

#define ALARM_BYTE_POS 118
inline void drawAlarm(bool on)
{
  uint8_t *bitmap = on ? alarmY : alarmN;
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
  uView.print(F("12 34"));

  if (millis() / 500 & 1)
  {
    uView.circleFill(20, 4, 2);
    uView.circleFill(20, 12, 2);
  }

  uView.setFontType(0);
  uView.setCursor(52, 0);
  uView.print(F("AM"));

  drawAlarm(millis() / 1000 & 1);
}

void setup()
{
  Serial.begin(112500);
  delay(1000);
  Serial.print(F("\033[2J\033[HAlarm Clock\r\n===========\r\n"));

  uView.begin();
  uView.contrast(0);
  uView.display();
  delay(1500);
}

void loop()
{
  uView.clear(PAGE);
  drawTime();
  uView.setCursor(0, 20);
  uView.print(F("2019-11-09"));
  uView.display();
}
