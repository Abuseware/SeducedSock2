#pragma once

#define BochsBreak() __asm__("xchg bx, bx")

void BochsPutc(char c);
void BochsPuts(char *str);
void BochsPuti(int64_t i);
