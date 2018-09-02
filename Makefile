AS = nasm
CC = gcc8

ASFLAGS = -Isrc/asm -I.
CFLAGS = -Isrc -Wall -Wextra -O0 -gdwarf -masm=intel -std=c11 -m64 -march=x86-64 -nostdlib -ffreestanding $(CFLAGS-$@)
LDFLAGS = -Wl,-O1,--nmagic -Telf64.ld

PY = python3.6


# File specific CFLAGS
CFLAGS-src/interrupt_handlers.o=-mgeneral-regs-only


# Targets
.PHONY: all

.SUFFIXES: .asm

src_c = $(wildcard src/*.c)
src_asm = $(wildcard src/asm/*.asm)
obj = $(src_c:.c=.o) $(src_asm:.asm=.o)
dep = $(obj:.o=.d)

.asm.o:
	$(AS) $(ASFLAGS) -f elf64 -o $@ $^

%.d: %.c
	$(CC) $(CFLAGS) -MM $< -MF $@

.txt.h:
	$(PY) rle.py $<

-include $(dep)

all: kernel live.iso

kernel:	$(obj)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

#kernel.sym: kernel
#	nm kernel | awk '($$2 == "T"){ print $$1" "$$3 }' > $@

live.iso: kernel grub.cfg
	rm -f $@
	install -d iso/boot/grub
	install grub.cfg iso/boot/grub/grub.cfg
	install kernel iso/kernel
	grub-mkrescue -o $@ iso

clean:
	rm -f kernel
	#rm -f kernel.sym
	rm -f $(obj)
	rm -f $(dep)
	rm -f live.iso
	find iso -mindepth 1 -type f -delete
	find iso -mindepth 1 -type d -delete

format: $(wildcard src/*.c) $(wildcard src/*.h)
	env ARTISTIC_STYLE_OPTIONS=.astylerc astyle -n $(wildcard src/*.c) $(wildcard src/*.h)

run: live.iso
	bochs -q || true

debug:
	tmux new-session -d 'gmake run'
	tmux split-window -h 'gdb'
	tmux attach -d

