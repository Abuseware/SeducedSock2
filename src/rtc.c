#include "rtc.h"
#include "port.h"

void RTCEnable(void) {
  //Enable RTC interrupt
  outb(0x21, inb(0x21) & ~1); //PIT
  outb(0x43, (2 << 1) | (3 << 4));
  outb(0x40, 59659 & 0xff);
  outb(0x40, (59659 >> 8) & 0xff);
}

void RTCDisable(void) {}
