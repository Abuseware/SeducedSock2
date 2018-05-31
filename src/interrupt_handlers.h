#pragma once

__attribute__ ((interrupt)) void interrupt_default(interrupt_frame_t __attribute__ ((unused)) *frame);
__attribute__ ((interrupt)) void interrupt_rtc(interrupt_frame_t __attribute__ ((unused)) *frame);
