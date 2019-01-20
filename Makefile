AS = yasm
CC = gcc

ASFLAGS = -Isrc/asm -I. -f elf64
CFLAGS = -Isrc -Wall -Wextra -Ofast -gdwarf -masm=intel -std=c11 -m64 -march=x86-64 -mtune=generic -mcmodel=large -nostdlib -ffreestanding -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS-$@)
LDFLAGS = -Wl,-O1,--nmagic -Telf64.ld -no-pie


# File specific CFLAGS
CFLAGS-obj/interrupt_handlers.o=-mgeneral-regs-only


# Targets
.PHONY: all objdir clean

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
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

kernel.sym: kernel
	@echo "[SYM]" $@
	@nm kernel | awk '($$2 == "T"){ print $$1" "$$3 }' > $@

$(isodir): kernel grub.cfg
	@echo "--- Preparing \"$(isodir)\" directory ---"
	@install -d $(isodir)/boot/grub
	@install grub.cfg $(isodir)/boot/grub/
	@install kernel $(isodir)/

live.iso: $(isodir)
	@echo "--- Building ISO ---"
	@rm -f $@
	@grub-mkrescue -o $@ --product-name="OS Dev" $(isodir)

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
	@docker run -it --rm -v ${PWD}:/root/osdev osdev
