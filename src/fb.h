#pragma once

typedef enum FramebufferColor {
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
} FramebufferColor_t;

void FramebufferInit(void);

void FramebufferClear(void);
void FramebufferScroll(void);

void FramebufferCursorShow(uint8_t start, uint8_t end);
void FramebufferCursorHide(void);
void FramebufferCursorMove(uint8_t x, uint8_t y);

void FramebufferWriteChr(uint8_t x, uint8_t y, char c, FramebufferColor_t fg, FramebufferColor_t bg);
void FramebufferWriteStr(uint8_t x, uint8_t y, char *str, FramebufferColor_t fg, FramebufferColor_t bg);

void FramebufferPutc(char c);
void FramebufferPuts(char *str);
void FramebufferPuti(int64_t i);
