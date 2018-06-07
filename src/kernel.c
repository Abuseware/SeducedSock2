#include <stdint.h>

#include <asm/port.h>
#include <pic.h>
#include <rtc.h>

#include <interrupt.h>
#include <interrupt_handlers.h>

#include <fb.h>
#include <util.h>

#include <multiboot.h>

uint8_t showLogo = 5;

void kmain() {
  //Reset if requirements not met
  if(!(mb_info->flags & (MULTIBOOT_INFO_BOOT_LOADER_NAME | MULTIBOOT_INFO_BOOTDEV | MULTIBOOT_INFO_CMDLINE | MULTIBOOT_INFO_FRAMEBUFFER_INFO))) {
    outb(0x64, 0xfe);
  }


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

  FramebufferInit();
  FramebufferClear();
  FramebufferCursorShow(11, 12);
  FramebufferCursorMove(0,0);
  FramebufferCursorHide();

  InterruptEnable();

  FramebufferPuts("Bootloader: ");
  FramebufferPuts((char *)(uint64_t)mb_info->boot_loader_name);
  FramebufferPutc('\n');

  FramebufferPuts("Boot device: hd(");
  FramebufferPuti(mb_info->boot_loader_name >> (8*3) & 0xff);
  FramebufferPuts("),part(");
  FramebufferPuti(mb_info->boot_loader_name >> (8*2) & 0xff);
  FramebufferPuts(")\n");

  FramebufferPuts("Cmdline: ");
  FramebufferPuts((char *)(uint64_t)mb_info->cmdline);
  FramebufferPutc('\n');


  FramebufferPuts("Framebuffer: ");
  FramebufferPuti(mb_info->framebuffer_width);
  FramebufferPuts("x");
  FramebufferPuti(mb_info->framebuffer_height);
  FramebufferPuts("@");
  FramebufferPuti(mb_info->framebuffer_bpp);
  FramebufferPuts(" &");
  FramebufferPuti(mb_info->framebuffer_addr);
  FramebufferPutc('\n');

  while(1){
    __asm__("hlt");
    //if(tick) {}
    //tick = 0;
  }
}
