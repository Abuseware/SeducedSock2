#include <stdint.h>
#include <stdbool.h>

#include "interrupt.h"
#include "interrupt_handlers.h"
#include "pic.h"

static uint64_t rtc_ticks = 0;
extern uint8_t counter;
extern bool tick;

__attribute__ ((interrupt)) void interrupt_default(interrupt_frame_t __attribute__ ((unused)) *frame) {
  PICSendEOI(0);
  PICSendEOI(1);
}


__attribute__ ((interrupt)) void interrupt_rtc(interrupt_frame_t __attribute__ ((unused)) *frame) {
  if((++rtc_ticks % 20) == 0) {
    counter++;
    tick = 1;
  }
  PICSendEOI(0);
}
