AS = nasm

#CC = clang
CC = gcc7
CFLAGS = -Wall -Wextra -Ofast -gdwarf -std=c11 -m64 -mgeneral-regs-only -nostdlib -ffreestanding

LDFLAGS = -Wl,-O1,--nmagic -Telf64.ld

PY = python3.6

.PHONY: all

.SUFFIXES: .txt .h .asm

src_c = $(wildcard src/*.c)
src_asm = $(wildcard src/*.asm)
obj = $(src_c:.c=.o) $(src_asm:.asm=.o)
dep = $(obj:.o=.d)

.asm.o:
	$(AS) $(ASFLAGS) -f elf64 -o $@ $^

%.d: %.c
	$(CC) $(CFLAGS) -MM $< -MF $@

.txt.h:
	$(PY) rle.py $<

all: kernel live.iso
-include $(dep)

kernel:	$(obj) src/logo.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
-include $(dep)

live.iso: kernel grub.cfg
	rm -f $@
	install -d iso/boot/grub
	install grub.cfg iso/boot/grub/grub.cfg
	install kernel iso/kernel
	grub-mkrescue -o $@ iso

clean:
	rm -f kernel
	rm -f $(obj)
	rm -f $(dep)
	rm -f live.iso
