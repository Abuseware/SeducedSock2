#include <asm/port.h>

#include <util.h>

#include <bochs.h>

void BochsPutc(char c) {
  outb(0xe9, c);
}

void BochsPuts(char *str) {
  uint32_t i = 0;
  while(str[i] != '\0') {
    BochsPutc(str[i]);
    i++;
  }
}

void BochsPuti(int64_t i) {
  char buf[21];
  itoa(i, buf);
  BochsPuts(buf);
}
