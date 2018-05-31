#include <stdint.h>

#include "interrupt.h"
#include "port.h"

typedef struct idt_descriptor {
  uint16_t addr_low;
  uint16_t segment;
  uint8_t zero;
  uint8_t flags;
  uint16_t addr_mid;
  uint32_t addr_high;
  uint32_t gap;
} __attribute__((packed)) idt_descriptor_t;

static idt_descriptor_t idt[256];

void InterruptInit(void) {
  struct {
    uint16_t length;
    void *base;
  } __attribute__((packed)) idtr = {(256 * 16) - 1, idt};

  __asm__("lidt %0" :: "m" (idtr));
}

void InterruptEnable(void) {
  __asm__("sti");
}

void InterruptDisable(void) {
  __asm__("cli");
}

void InterruptSetDescriptor(uint8_t index, uint8_t segment, void *func) {
  union {
    void* ptr;
    struct {
      uint16_t low;
      uint16_t mid;
      uint32_t high;
    } __attribute__((packed)) part;
  } ptr;


  ptr.ptr = func;

  idt[index].addr_low = ptr.part.low;
  idt[index].segment = segment;
  idt[index].zero = 0;
  idt[index].flags = 0x8e;
  idt[index].addr_mid = ptr.part.mid;
  idt[index].addr_high = ptr.part.high;
  idt[index].gap = 0;
}
