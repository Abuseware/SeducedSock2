#include <stdint.h>

#include <fb.h>
#include <asm/port.h>

#include <util.h>

#include <multiboot.h>

#define COLOR_FG LightGreen
#define COLOR_BG Black

static uint16_t *fb;
static uint32_t fb_w;
static uint32_t fb_h;

static uint8_t current_x;
static uint8_t current_y;

void FramebufferInit(void) {
  fb = (uint16_t *)mb_info->framebuffer_addr;
  fb_w = mb_info->framebuffer_width;
  fb_h = mb_info->framebuffer_height;
}

void FramebufferClear(void) {
  for(uint8_t y = 0; y < fb_h - 1; y++) {
    for(uint8_t x = 0; x < fb_w - 1; x++) {
      FramebufferWriteChr(x, y, ' ', COLOR_FG, COLOR_BG);
    }
  }
  current_x = 0;
  current_y = 0;
}

void FramebufferScroll(void) {
  for(uint8_t y = 0; y < fb_h - 1; y++) {
    for(uint8_t x = 0; x < fb_w; x++) {
      fb[(x) + (y * fb_w)] = fb[x + ((y + 1) * fb_w)];
    }
  }
  for(uint8_t x = 0; x < fb_w; x++) {
    FramebufferWriteChr(x, fb_h - 1, ' ', COLOR_FG, COLOR_BG);
  }
}

void FramebufferCursorShow(uint8_t start, uint8_t end)
{
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3E0) & 0xE0) | end);
}

void FramebufferCursorHide(void) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void FramebufferCursorMove(uint8_t x, uint8_t y) {
  uint16_t pos = y * fb_w + x;
  outb(0x3D4, 0xf);
  outb(0x3D5, (uint8_t)(pos & 0xff));

  outb(0x3D4, 0xe);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xff));
}

void FramebufferWriteChr(uint8_t x, uint8_t y, char c, FramebufferColor_t fg, FramebufferColor_t bg) {
  fb[x + (y * fb_w)] = c | (((bg << 4) | (fg & 0x0f)) << 8);
}

void FramebufferWriteStr(uint8_t x, uint8_t y, char *str, FramebufferColor_t fg, FramebufferColor_t bg) {
  uint64_t i = 0;
  while(*str != '\0') {
    FramebufferWriteChr(x + i, y, *str++, fg, bg);
    i++;
  }
}

void FramebufferPutc(char c) {
  if(current_x >= fb_w) {
    current_x = 0;
    current_y++;
  }

  if(current_y >= fb_h) {
    FramebufferScroll();
    current_y = fb_h - 1;
  }

  switch(c) {
    case '\n': {
      current_x = 0;
      current_y++;
      break;
    }
    case '\r': {
      current_x = 0;
      break;
    }
    default: {
      FramebufferWriteChr(current_x++, current_y, c, COLOR_FG, COLOR_BG);
      break;
    }
  }
  FramebufferCursorMove(current_x, current_y);
}

void FramebufferPuts(char *str) {
  while(*str != '\0') {
    FramebufferPutc(*str++);
  }
}

void FramebufferPuti(int64_t i) {
  char buf[21];
  itoa(i, buf);
  FramebufferPuts(buf);
}
