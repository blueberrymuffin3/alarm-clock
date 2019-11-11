#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include <stdint.h>

struct __attribute__ ((packed)) PacketMVStatus
{
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;

  bool isAlarmOn = false;

  float tempF = 0./0.;
  float humidity = 0./0.;
};
#endif
