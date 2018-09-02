#pragma once

#define putc(x) VGAPutc(x)
#define puts(x) VGAPuts(x)
#define puti(x) VGAPuti(x)
#define puth(x) VGAPuth(x)

typedef enum VGAColor {
  VGA_Black = 0,
  VGA_Blue = 0x000000ff,
  VGA_Green = 0x0000ff00,
  VGA_Red = 0x00ff0000,
  VGA_White = 0xffffff
} VGAColor_t;

void VGAInit(void);

void VGASetTextColor(VGAColor_t color);

void VGAPutPixel(uint64_t x, uint64_t y, VGAColor_t color);

void VGAPutc(char c);
void VGAPuts(char *str);
void VGAPuti(int64_t i);
void VGAPuth(int64_t i);
