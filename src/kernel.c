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
#include "image.c"

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

  VGAInit();
  VGASetTextColor(VGA_Black);

  InterruptEnable();


  BochsPuts("Bootloader: ");
  BochsPuts(mb_loader->string);
  BochsPutc('\n');

  for(unsigned int y = 0; y < gimp_image.height; y++)
    for(unsigned int x = 0; x < gimp_image.width; x++) {
      register uint32_t pixel = gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel] << 16;
      pixel |= gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel + 1] << 8;
      pixel |= gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel + 2];
      VGAPutPixel(x, y, pixel);
    }

  puts("Hello World!");



  while(1) {
    __asm__("hlt");
    //if(tick) {}
    //tick = 0;
  }
}
