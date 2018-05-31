#include <stdint.h>

#include "fb.h"
#include "port.h"

#define FB_W 80
#define FB_H 25

static unsigned char *fb = (unsigned char*)0xB8000;

void FramebufferClear(void) {
  for(uint8_t y = 0; y < FB_H - 1; y++) {
    for(uint8_t x = 0; x < FB_W - 1; x++) {
      fb[(x * 2) + (y * FB_W * 2)] = 0x20;
      fb[(x * 2) + (y * FB_W * 2) + 1] = 0xA;
    }
  }
}

void FramebufferSetStyle(FramebufferColor_t fg, FramebufferColor_t bg) {
  for(uint8_t y = 0; y < FB_H; y++) {
    for(uint8_t x = 0; x < FB_W; x++) {
      fb[(x * 2) + (y * FB_W * 2) + 1] = (bg << 4) | (fg & 0x0f);
    }
  }
}

void FramebufferCursorHide(void) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void FramebufferCursorMove(uint8_t x, uint8_t y) {
  outb(0x3D4, 0xe);
  outb(0x3D5, x);

  outb(0x3D4, 0xf);
  outb(0x3D5, y);
}

void FramebufferWriteChr(uint8_t x, uint8_t y, char c, FramebufferColor_t fg, FramebufferColor_t bg) {
  fb[(x * 2) + (y * FB_W * 2)] = c;
  fb[(x * 2) + (y * FB_W * 2) + 1] = (bg << 4) | (fg & 0x0f);
}
void FramebufferWriteStr(uint8_t x, uint8_t y, char *str, FramebufferColor_t fg, FramebufferColor_t bg) {
  uint32_t i = 0;
  while(str[i] != '\0') {
    FramebufferWriteChr(x + i, y, str[i], fg, bg);
    ++i;
  }
}
