#include <stdint.h>
#include <stddef.h>

#include <asm/port.h>
#include <pic.h>
#include <rtc.h>

#include <interrupt.h>
#include <interrupt_handlers.h>

#include <fb.h>

#include <bochs.h>
#include <util.h>

#include <multiboot.h>

struct multiboot_header mb_hdr __attribute__((section (".multiboot"), unused)) = {
  .magic = MULTIBOOT_HEADER_MAGIC,
  .flags = MULTIBOOT_PAGE_ALIGN,
  .checksum = -(MULTIBOOT_HEADER_MAGIC  + (MULTIBOOT_PAGE_ALIGN)),
  .mode_type = 1
};

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

  InterruptEnable();

  BochsPuts("Bootloader: ");
  BochsPuts((char *)(uint64_t)mb_info->boot_loader_name);
  BochsPutc('\n');

  FBInit();
  FBClear();
  FBCursorHide();

  FBPuts("Hello World!\n");

  while(1) {
    __asm__("hlt");
    if(tick) {
      FBPuti(counter);
      FBPuts(". Hello!\n");
      tick = 0;
    }
  }
}
