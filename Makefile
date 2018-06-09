AS = nasm

#CC = clang
CC = gcc7
CFLAGS = -Isrc -Wall -Wextra -Ofast -gdwarf -masm=intel -std=c11 -m64 -nostdlib -ffreestanding
ifeq ($(CC),clang)
	CFLAGS += -mcpu x86_64 -target x86_64-pc-none-elf
else
	CFLAGS += -march=x86-64 -mgeneral-regs-only
endif

LDFLAGS = -Wl,-O1,--nmagic -Telf64.ld

PY = python3.6

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

all: kernel live.iso
-include $(dep)

kernel:	$(obj)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
-include $(dep)

kernel.sym: kernel
	nm kernel | awk '($$2 == "T"){ print $$1" "$$3 }' > $@

live.iso: kernel grub.cfg grub_efi.cfg
	rm -f $@
	install -d iso/boot/grub
	install -d iso/EFI/BOOT
	install grub.cfg iso/boot/grub/grub.cfg
	install kernel iso/kernel
	grub-mkstandalone -O x86_64-efi -o iso/EFI/BOOT/BOOTX64.EFI "boot/grub/grub.cfg=grub_efi.cfg"
	grub-mkrescue -o $@ iso

clean:
	rm -f kernel
	rm -f kernel.sym
	rm -f $(obj)
	rm -f $(dep)
	rm -f live.iso
	find iso -mindepth 1 -type f -delete
	find iso -mindepth 1 -type d -delete

run: live.iso kernel.sym
	bochs -q -rc .bochsscript
