#include <stdint.h>

#include <interrupt.h>
#include <interrupt_handlers.h>
#include <pic.h>

static uint64_t rtc_ticks = 0;

//Default exception handler
__attribute__ ((interrupt)) void interrupt_default(interrupt_frame_t __attribute__ ((unused)) *frame) {
  PICSendEOI(0);
  PICSendEOI(1);
}

//CPU exceptions
__attribute__ ((interrupt)) void interrupt_cpu_div_by_zero(interrupt_frame_t __attribute__ ((unused)) *frame) {
  exceptions |= 1;
}

__attribute__ ((interrupt)) void interrupt_cpu_overflow(interrupt_frame_t __attribute__ ((unused)) *frame) {
  exceptions |= 1 << 4;
}

__attribute__ ((interrupt)) void interrupt_rtc(interrupt_frame_t __attribute__ ((unused)) *frame) {
  if((++rtc_ticks % 20) == 0) {
    counter++;
    tick = 1;
  }
  exceptions |= (uint64_t)1 << 0x20;
  PICSendEOI(0);
}
