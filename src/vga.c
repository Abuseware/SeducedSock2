#include <stdint.h>

#include <multiboot.h>

#include <util.h>
#include <bochs.h>

#include <vga.h>

typedef struct VBE_MIB {
  uint16_t attributes;
  uint8_t winA,winB;
  uint16_t granularity;
  uint16_t winsize;
  uint16_t segmentA, segmentB;
  uint32_t realFctPtr;
  uint16_t pitch; // bytes per scanline

  uint16_t Xres, Yres;
  uint8_t Wchar, Ychar, planes, bpp, banks;
  uint8_t memory_model, bank_size, image_pages;
  uint8_t reserved0;

  uint8_t red_mask, red_position;
  uint8_t green_mask, green_position;
  uint8_t blue_mask, blue_position;
  uint8_t rsv_mask, rsv_position;
  uint8_t directcolor_attributes;

  uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
  uint32_t reserved1;
  uint16_t reserved2;
} __attribute__((packed)) VBE_MIB_t;

static VBE_MIB_t *vbe_mib;
static uint16_t vga_w, vga_h, vga_pitch;
static uint8_t vga_bpp;
static volatile uint8_t *vga;

void VGAInit(void) {
  //vga = (uint16_t*)(uint64_t)mb_info->framebuffer_addr;
  vga = (uint8_t*)0xa0000;
  vbe_mib = (VBE_MIB_t*)(uint64_t)mb_info->vbe_mode_info;
  vga_w = mb_info->framebuffer_width;
  vga_h = mb_info->framebuffer_height;
  vga_bpp = mb_info->framebuffer_bpp;
  vga_pitch = vbe_mib->pitch;

  BochsPuts("VGA: ");
  BochsPuti(vga_w);
  BochsPutc('x');
  BochsPuti(vga_h);
  BochsPutc('@');
  BochsPuti(vga_bpp);
  BochsPuts(" Pitch: ");
  BochsPuti(vga_pitch);
  BochsPutc('\n');
}

void VGAPutPixel(uint64_t x, uint64_t y, uint8_t rgb) {
  vga[(y * vga_pitch) + x] = rgb;
}
