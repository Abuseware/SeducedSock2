AS = yasm
CC = gcc

ASFLAGS = -Isrc/asm -I. -f elf64
CFLAGS = -Isrc -Wall -Wextra -O1 -gdwarf -masm=intel -std=c11 -m64 -march=x86-64 -nostdlib -ffreestanding $(CFLAGS-$@)
LDFLAGS = -Wl,-O1,--nmagic -Telf64.ld -no-pie


# File specific CFLAGS
CFLAGS-obj/interrupt_handlers.o=-mgeneral-regs-only


# Targets
.PHONY: all objdir

srcdir = src
objdir = obj
isodir = iso

src_c = $(wildcard $(srcdir)/*.c)
src_s = $(wildcard $(srcdir)/asm/*.s)
obj_c = $(patsubst $(srcdir)/%,$(objdir)/%,$(src_c:.c=.o))
obj_s = $(patsubst $(srcdir)/asm/%,$(objdir)/%,$(src_s:.s=.o))

$(objdir)/%.o: $(srcdir)/asm/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(objdir)/%.o: $(srcdir)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(objdir) kernel live.iso

$(objdir):
	install -d $(objdir)

kernel: $(obj_s) $(obj_c)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

live.iso: kernel grub.cfg
	rm -f $@
	install -d $(isodir)/boot/grub
	install grub.cfg $(isodir)/boot/grub/grub.cfg
	install kernel $(isodir)/kernel
	grub-mkrescue -o $@ $(isodir)

clean:
	rm -f kernel || true
	rm -f live.iso || true
	rm -rf $(objdir) || true
	rm -rf $(isodir) || true

run: live.iso
	bochs -q || true

docker-image: Dockerfile
	docker build -t osdev .

docker-build:
	docker run -t --rm -v ${PWD}:/root/osdev osdev
