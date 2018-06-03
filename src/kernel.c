#include <stdint.h>

#include "port.h"
#include "pic.h"
#include "rtc.h"

#include "interrupt.h"
#include "interrupt_handlers.h"

#include "fb.h"
#include "logo.h"

void kmain() {

  //Disable NMI
  outb(0x70, inb(0x70) | 0x80);


  PICInit();
  PICDisable();

  RTCEnable();

  InterruptInit();


  for(uint16_t i = 0; i < 256; i++) {
    InterruptSetDescriptor(i, 0x8, interrupt_default);
  }

  InterruptSetDescriptor(0x0, 0x8, interrupt_cpu_div_by_zero);
  InterruptSetDescriptor(0x4, 0x8, interrupt_cpu_overflow);
  InterruptSetDescriptor(0x20, 0x8, interrupt_rtc);


  FramebufferClear();
  FramebufferCursorMove(0,0);
  FramebufferCursorHide();

  for(uint32_t y = 0; y < logo_h; y++) {
    for(uint32_t x = 0; x < logo_w; x++) {
      FramebufferWriteChr(logo_x + x, logo_y + y, logo[y][x], Black, Black);
    }
  }

  InterruptEnable();
  while(1){
    __asm__("hlt");
    if(tick) {
      FramebufferSetStyle((counter % 0xe) + 1, 0);
      FramebufferWriteStr(0, 0, "Ticks:", White, Black);
      FramebufferWriteChr(7, 0, 0x30 + (counter / 100 % 10), White, Black);
      FramebufferWriteChr(8, 0, 0x30 + (counter / 10 % 10), White, Black);
      FramebufferWriteChr(9, 0, 0x30 + (counter % 10), White, Black);
      tick = 0;
    }

    FramebufferWriteStr(0, 24, "Errors:", Red, Black);
    for(uint8_t i = 0; i < 32; i++) {
      if(exceptions & (1 << i)) {
        FramebufferWriteChr(8 + i, 24, '1', Red, Black);
      } else {
        FramebufferWriteChr(8 + i, 24, '0', Green, Black);
      }
    }
  }
}
