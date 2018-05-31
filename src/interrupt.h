#pragma once

typedef struct interrupt_frame {
  uint64_t ip;
  uint64_t cs;
  uint64_t flags;
  uint64_t sp;
  uint64_t ss;
} interrupt_frame_t;

void InterruptInit(void);

void InterruptEnable(void);
void InterruptDisable(void);

void InterruptSetDescriptor(uint8_t index, uint8_t segment, void *func);
