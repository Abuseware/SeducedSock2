[cpu x64]
[bits 64]

global inb
global outb

global inw
global outw

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

outw:
  mov rax, rsi
  mov rdx, rdi
  out dx, ax
  ret

inw:
  mov rdx, rdi
  in ax, dx
  ret
