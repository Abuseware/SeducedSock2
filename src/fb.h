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

void FramebufferClear(void);
void FramebufferSetStyle(FramebufferColor_t fg, FramebufferColor_t bg);
void FramebufferCursorHide(void);
void FramebufferCursorMove(uint8_t x, uint8_t y);

void FramebufferWriteChr(uint8_t x, uint8_t y, char c, FramebufferColor_t fg, FramebufferColor_t bg);
void FramebufferWriteStr(uint8_t x, uint8_t y, char *str, FramebufferColor_t fg, FramebufferColor_t bg);
