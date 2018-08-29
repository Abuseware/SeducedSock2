#pragma once

#define BochsBreak() __asm__("xchg bx, bx")

void BochsPutc(char c);
void BochsPuts(char *str);
void BochsPuti(int64_t i);
void BochsPuth(int64_t i);

void BochsVGAWrite(uint16_t index, uint16_t data);
uint16_t BochsVGARead(uint16_t index);
