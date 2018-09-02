#include <stdint.h>
#include <stddef.h>

#include <asm/port.h>
#include <pic.h>
#include <rtc.h>

#include <interrupt.h>
#include <interrupt_handlers.h>

//#include <fb.h>
#include <vga.h>

#include <bochs.h>
#include <util.h>

#include <multiboot2.h>
#include "image.xbm"

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

  MultibootInit();

  struct multiboot_tag_string *mb_loader = (struct multiboot_tag_string *)MultibootGetTag(MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME);
  struct multiboot_tag_bootdev *mb_bootdev = (struct multiboot_tag_bootdev *)MultibootGetTag(MULTIBOOT_TAG_TYPE_BOOTDEV);
  struct multiboot_tag_framebuffer *mb_fb = (struct multiboot_tag_framebuffer *)MultibootGetTag(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

  VGAInit();
  VGASetTextColor(VGA_Red);

  InterruptEnable();


  BochsPuts("Bootloader: ");
  BochsPuts(mb_loader->string);
  BochsPutc('\n');

  //puts("Hello!");

  for(unsigned int y = 0; y < image_height; y++)
    for(unsigned int x = 0; x < image_width / 8; x++)
      for(unsigned int p = 0; p < 8; p++) {
        VGAPutPixel((x * 8) + p, y, image_bits[(y * image_width / 8) + x] & 1 << p ? 0 : 0xffffffff);
      }

  puts("Hello!");



  while(1) {
    __asm__("hlt");
    //if(tick) {}
    //tick = 0;
  }
}
