#include <stdint.h>

#include <fb.h>
#include <asm/port.h>

#include <util.h>
#include <bochs.h>

#include <multiboot2.h>

#define COLOR_FG FB_Foreground
#define COLOR_BG FB_Background

uint32_t FBColorPalette[5] = {
  0x232525,
  0x323232,
  0x808080,
  0xa9b7c6,
  0x9876aa
};

static uint16_t *fb;
static uint32_t fb_w;
static uint32_t fb_h;

static uint8_t current_x;
static uint8_t current_y;

static uint8_t current_fg = COLOR_FG;
static uint8_t current_bg = COLOR_BG;

void FBInit(void) {
  struct multiboot_tag_framebuffer *fb_info = (struct multiboot_tag_framebuffer *)MultibootGetTag(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);
  fb = (uint16_t *)fb_info->common.framebuffer_addr;
  fb_w = fb_info->common.framebuffer_width;
  fb_h = fb_info->common.framebuffer_height;

  //Set palette, divide hex values by 4 for mapping (256 -> 64 colors)
  for(uint8_t c = 0; c < 5; c++) {
    outb(0x03C8, c);
    outb(0x03C9, (FBColorPalette[c] >> 16 & 0xff) / 4);
    outb(0x03C9, (FBColorPalette[c] >> 8 & 0xff) / 4);
    outb(0x03C9, (FBColorPalette[c] & 0xff) / 4);
  }
}

void FBClear(void) {
  for(uint8_t y = 0; y < fb_h - 1; y++) {
    for(uint8_t x = 0; x < fb_w - 1; x++) {
      FBWriteChr(x, y, ' ', COLOR_FG, COLOR_BG);
    }
  }

  current_x = 0;
  current_y = 0;
}

void FBScroll(void) {
  for(uint8_t y = 0; y < fb_h - 1; y++) {
    for(uint8_t x = 0; x < fb_w; x++) {
      fb[(x) + (y * fb_w)] = fb[x + ((y + 1) * fb_w)];
    }
  }

  for(uint8_t x = 0; x < fb_w; x++) {
    FBWriteChr(x, fb_h - 1, ' ', COLOR_FG, COLOR_BG);
  }
}

void FBCursorShow(uint8_t start, uint8_t end) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3E0) & 0xE0) | end);
}

void FBCursorHide(void) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void FBCursorMove(uint8_t x, uint8_t y) {
  uint16_t pos = y * fb_w + x;
  outb(0x3D4, 0xf);
  outb(0x3D5, (uint8_t)(pos & 0xff));

  outb(0x3D4, 0xe);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xff));
}

void FBSetColors(FBColor_t fg, FBColor_t bg) {
  current_fg = fg;
  current_bg = bg;
}

void FBWriteChr(uint8_t x, uint8_t y, char c, FBColor_t fg, FBColor_t bg) {
  fb[x + (y * fb_w)] = c | (((bg << 4) | (fg & 0x0f)) << 8);
}

void FBWriteStr(uint8_t x, uint8_t y, char *str, FBColor_t fg, FBColor_t bg) {
  uint64_t i = 0;

  while(*str != '\0') {
    FBWriteChr(x + i, y, *str++, fg, bg);
    i++;
  }
}

void FBPutc(char c) {
  if(current_x >= fb_w) {
    current_x = 0;
    current_y++;
  }

  if(current_y >= fb_h) {
    FBScroll();
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
        FBWriteChr(current_x++, current_y, c, current_fg, current_bg);
        break;
      }
  }

  FBCursorMove(current_x, current_y);
}

void FBPuts(char *str) {
  while(*str != '\0') {
    FBPutc(*str++);
  }
}

void FBPuti(int64_t i) {
  char buf[21];
  itoa(i, buf);
  FBPuts(buf);
}

void FBPuth(int64_t i) {
  char buf[23];
  itoh(i, buf);
  FBPuts(buf);
}
