#pragma once

#include <stdint.h>

__attribute__((naked)) extern uint8_t inb(uint16_t port);
__attribute__((naked)) extern void outb(uint16_t port, uint8_t data);

__attribute__((naked)) extern uint16_t inw(uint16_t port);
__attribute__((naked)) extern void outw(uint16_t port, uint16_t data);
