#include <stdint.h>

#include <multiboot.h>
#include <asm/port.h>

#include <util.h>
#include <bochs.h>
#include <bochs_vga.h>

#include <vga.h>
#include <vga_font.h>

#define MARGIN 8

static uint32_t *vga;
static uint16_t vga_w, vga_h, vga_pitch;
static uint8_t vga_bpp;

static uint32_t current_x;
static uint32_t current_y;

static uint32_t text_color = 0xffffffff;

void VGAInit(void) {
  vga = (uint32_t *)mb_info->framebuffer_addr;
  vga_w = mb_info->framebuffer_width;
  vga_h = mb_info->framebuffer_height;
  vga_bpp = mb_info->framebuffer_bpp;
  vga_pitch = mb_info->framebuffer_pitch;

  /*if(BochsVGARead(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID5) {
    BochsVGAWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BochsVGAWrite(VBE_DISPI_INDEX_XRES, vga_w);
    BochsVGAWrite(VBE_DISPI_INDEX_YRES, vga_h);
    BochsVGAWrite(VBE_DISPI_INDEX_BPP, vga_bpp);

    BochsVGAWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

    BochsPuts("BOCHS detected, enabling LFB VGA\n");
    //vga = (uint32_t *)VBE_DISPI_LFB_PHYSICAL_ADDRESS;
  }*/

  BochsPuts("VGA: ");
  BochsPuti(vga_w);
  BochsPutc('x');
  BochsPuti(vga_h);
  BochsPutc('@');
  BochsPuti(vga_bpp);
  BochsPuts(" Pitch: ");
  BochsPuti(vga_pitch);
  BochsPuts(" Address: ");
  BochsPuth((uint64_t)vga);
  BochsPutc('\n');
}

void VGASetTextColor(uint32_t color) {
  text_color = color;
}

void VGAPutPixel(uint64_t x, uint64_t y, uint32_t color) {
  vga[(y * vga_pitch / (vga_bpp / 8)) + x] = color;
}

void VGAPutc(char c) {
  uint8_t buf;

  switch(c) {
    case '\n':
      current_x = 0;
      current_y++;
      break;

    case '\r':
      current_x = 0;
      break;
  }

  if(c < 0x20 || c > 0x7e) {
    return;
  }


  if(current_x >= ((uint32_t)vga_w - (2 * MARGIN)) / 8) {
    current_x = 0;
    current_y++;
  }

  if(current_y >= ((uint32_t)vga_h - (2 * - MARGIN)) / 16) {
    //TODO: Scrolling!
    current_y = 0;
  }

  for(uint8_t y = 0; y < 13; y++) {
    buf = vga_font[c - 0x20][12 - y];

    for(uint8_t x = 0; x < 8; x++) {
      if(buf >> (7 - x) & 1) {
        VGAPutPixel(MARGIN + current_x * 10 + x, MARGIN + current_y * 16 + y, text_color);
      }
    }
  }

  current_x++;
}

void VGAPuts(char *str) {
  while(*str != '\0') {
    VGAPutc(*str++);
  }
}

void VGAPuti(int64_t i) {
  char buf[21];
  itoa(i, buf);
  VGAPuts(buf);
}

void VGAPuth(int64_t i) {
  char buf[23];
  itoa(i, buf);
  VGAPuts(buf);
}
