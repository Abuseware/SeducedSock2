#pragma once

uint8_t counter;
uint8_t tick;

uint64_t exceptions;

__attribute__ ((interrupt)) void interrupt_default(interrupt_frame_t __attribute__ ((unused)) *frame);

__attribute__ ((interrupt)) void interrupt_cpu_div_by_zero(interrupt_frame_t __attribute__ ((unused)) *frame);
__attribute__ ((interrupt)) void interrupt_cpu_overflow(interrupt_frame_t __attribute__ ((unused)) *frame);

__attribute__ ((interrupt)) void interrupt_rtc(interrupt_frame_t __attribute__ ((unused)) *frame);
