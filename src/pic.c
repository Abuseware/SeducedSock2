#include <stdint.h>

#include <pic.h>
#include <asm/port.h>

void PICInit(void) {
  //Reset PIC
  outb(0x20, 0x11);
  outb(0xa0, 0x11);

  //Remap PIC
  outb(0x21, 0x20); //PIC1 @32
  outb(0xa1, 0x28); //PIC2 @40

  //Set PIC cascade
  outb(0x21, 0x4);
  outb(0xa1, 0x2);

  //Set PIC ICW4
  outb(0x21, 0x1);
  outb(0xa1, 0x1);
}

void PICEnable(void) {}

void PICDisable(void) {
  //Disable PIC interrupts
  outb(0x21, 0xff);
  outb(0xa1, 0xff);
}

void PICSendEOI(uint8_t n) {
  switch(n) {
    case 0:
      outb(0x20, 0x20);
      break;
    case 1:
      outb(0xa0, 0x20);
      break;
  }
}
