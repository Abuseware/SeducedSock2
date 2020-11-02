AS = yasm
CC = clang
LD = ld.gold

ASFLAGS = -Isrc/asm -I. -f elf64
CFLAGS = -Isrc -Wall -Wextra -O2 -gdwarf -masm=intel -std=c11 --target=x86_64-none-elf -m64 -march=x86-64 -mtune=generic -mcmodel=large -nostdlib -ffreestanding -fno-PIC -fno-PIE -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS-$@)
LDFLAGS = -O1 --nmagic -no-pie -no-pic -Telf64.ld


# File specific CFLAGS
CFLAGS-obj/interrupt_handlers.o=-mgeneral-regs-only


# Targets
.PHONY: all objdir clean

# Directories
syslinux = /usr/lib/syslinux/modules/bios

srcdir = src
objdir = obj
isodir = iso

src_c = $(wildcard $(srcdir)/*.c)
src_s = $(wildcard $(srcdir)/asm/*.s)
obj_c = $(patsubst $(srcdir)/%,$(objdir)/%,$(src_c:.c=.o))
obj_s = $(patsubst $(srcdir)/%,$(objdir)/%,$(src_s:.s=.o))

$(objdir)/%.o: $(srcdir)/%.s
	@echo "[AS]" $<
	@$(AS) $(ASFLAGS) -o $@ $<

$(objdir)/%.o: $(srcdir)/%.c
	@echo "[CC]" $<
	@$(CC) $(CFLAGS) -c -o $@ $<

all: $(objdir) build-kernel kernel kernel.sym live.iso
	@echo "--- DONE! ---"

$(objdir):
	@echo "--- Preparing \"$(objdir)\" directory ---"
	@mkdir `find src -type d | sed 's:^src:obj:'`

build-kernel:
	@echo "--- Building kernel ---"

kernel: $(obj_s) $(obj_c)
	@echo "[LINK]" $@
	@$(LD) $(LDFLAGS) -o $@ $^

kernel.sym: kernel
	@echo "[SYM]" $@
	@nm kernel | awk '($$2 == "T"){ print $$1" "$$3 }' > $@

$(isodir): kernel grub.cfg syslinux.cfg
	@echo "--- Preparing \"$(isodir)\" directory ---"

	@install -d $(isodir)/boot/syslinux

	@install syslinux.cfg $(isodir)/boot/syslinux/

	@install /usr/lib/ISOLINUX/isolinux.bin $(isodir)/boot/syslinux/
	@install $(syslinux)/ldlinux.c32 $(isodir)/boot/syslinux/
	@install $(syslinux)/libcom32.c32 $(isodir)/boot/syslinux/
	
	@install $(syslinux)/mboot.c32 $(isodir)/boot/syslinux/

	@install kernel $(isodir)/

live.iso: $(isodir)
	@echo "--- Building ISO ---"
	mkisofs -o $@ -b boot/syslinux/isolinux.bin -c boot/syslinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table $(isodir)

clean:
	@echo "--- Cleaning ---"
	@rm -f kernel || true
	@rm -f *.iso || true
	@rm -rf $(objdir) || true
	@rm -rf $(isodir) || true

run:
	@echo "--- Starting BOCHS ---"
	@bochs -q -rc .bochsscript || true

docker-image: Dockerfile
	@echo "--- Building Docker image ---"
	@docker build -t osdev .

docker-build:
	@echo "--- Starting Docker build ---"
	@docker run -t --rm -v ${PWD}:/root/osdev osdev
