#pragma once

#define putc(x) VGAPutc(x)
#define puts(x) VGAPuts(x)
#define puti(x) VGAPuti(x)
#define puth(x) VGAPuth(x)

typedef enum VGAColor {
  Black,
  Blue,
  Green,
  Cyan,
  Red,
  Magenta,
  Brown,
  LightGray,
  Gray,
  LightBlue,
  LightGreen,
  LightCyan,
  LightRed,
  LightMagenta,
  Yellow,
  White
} VGAColor_t;

void VGAInit(void);

void VGAPutPixel(uint64_t x, uint64_t y, uint32_t color);

void VGAPutc(char c);
void VGAPuts(char *str);
void VGAPuti(int64_t i);
void VGAPuth(int64_t i);
