#pragma once

//#define putc(x) FBPutc(x)
//#define puts(x) FBPuts(x)
//#define puti(x) FBPuti(x)
//#define puth(x) FBPuth(x)

#include <stdint.h>

typedef enum FBColor {
  FB_Background,
  FB_BackgroundHighlight,
  FB_Comment,
  FB_Foreground,
  FB_Emphasised,
} FBColor_t;

uint32_t FBColorPalette[5];

/*typedef enum FBColor {
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
} FBColor_t;*/

void FBInit(void);

void FBClear(void);
void FBScroll(void);

void FBCursorShow(uint8_t start, uint8_t end);
void FBCursorHide(void);
void FBCursorMove(uint8_t x, uint8_t y);

void FBSetColors(FBColor_t fg, FBColor_t bg);

void FBWriteChr(uint8_t x, uint8_t y, char c, FBColor_t fg, FBColor_t bg);
void FBWriteStr(uint8_t x, uint8_t y, char *str, FBColor_t fg, FBColor_t bg);

void FBPutc(char c);
void FBPuts(char *str);
void FBPuti(int64_t i);
void FBPuth(int64_t i);
