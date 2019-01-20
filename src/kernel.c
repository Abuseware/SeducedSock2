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

#include <multiboot.h>
#include "image.h"

struct multiboot_header mb_hdr __attribute__((section (".multiboot"), unused)) = {
  .magic = MULTIBOOT_HEADER_MAGIC,
  .flags = MULTIBOOT_PAGE_ALIGN | MULTIBOOT_VIDEO_MODE,
  .checksum = -(MULTIBOOT_HEADER_MAGIC  + (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_VIDEO_MODE)),

  .mode_type = 0,
  .width = 800,
  .height = 600,
  .depth = 32
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

  if(!(mb_info->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
    BochsPuts("Framebuffer info not provided\n");
    return;
  }

  BochsPuts("Framebuffer type: ");
  BochsPuti(mb_info->framebuffer_type);
  BochsPutc('\n');

  VGAInit();
  VGASetTextColor(VGA_Black);

  for(unsigned int y = 0; y < gimp_image.height; y++)
    for(unsigned int x = 0; x < gimp_image.width; x++) {
      VGAPutPixel(x, y,
        gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel] << 16 | \
        gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel + 1] << 8 | \
        gimp_image.pixel_data[(y * gimp_image.width + x) * gimp_image.bytes_per_pixel + 2]
      );
    }

  puts("Hello World!");



  while(1) {
    __asm__("hlt");
    //if(tick) {}
    //tick = 0;
  }
}
