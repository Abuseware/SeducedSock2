#include <asm/port.h>

#include <util.h>

#include <bochs.h>
#include <bochs_vga.h>

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

void BochsPuth(int64_t i) {
  char buf[23];
  itoh(i, buf);
  BochsPuts(buf);
}

void BochsVGAWrite(uint16_t index, uint16_t data) {
  outw(VBE_DISPI_IOPORT_INDEX, index);
  outw(VBE_DISPI_IOPORT_DATA, data);
}

uint16_t BochsVGARead(uint16_t index) {
  outw(VBE_DISPI_IOPORT_INDEX, index);
  return inw(VBE_DISPI_IOPORT_DATA);
}
