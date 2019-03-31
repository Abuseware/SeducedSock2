#include <asm/port.h>

#include <util.h>

#include <bochs.h>

__attribute__((inline, always_inline)) static uint8_t check_bochs(void) {
  return (inb(0xe9) == 0xe9);
}

void BochsPutc(char c) {
  if(!check_bochs()) return;
  outb(0xe9, c);
}

void BochsPuts(char *str) {
  if(!check_bochs()) return;
  uint32_t i = 0;

  while(str[i] != '\0') {
    BochsPutc(str[i]);
    i++;
  }
}

void BochsPuti(int64_t i) {
  if(!check_bochs()) return;
  char buf[21];
  itoa(i, buf);
  BochsPuts(buf);
}

void BochsPuth(int64_t i) {
  if(!check_bochs()) return;
  char buf[23];
  itoh(i, buf);
  BochsPuts(buf);
}
