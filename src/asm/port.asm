[cpu x64]
[bits 64]

global inb
global outb

[segment .text]
outb:
  mov rax, rsi
  mov rdx, rdi
  out dx, al
  ret

inb:
  mov rdx, rdi
  in al, dx
  ret
