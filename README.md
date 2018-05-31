# Stupid OS development attempt

Barely working x86_64 "operating system"

## How to build
Simply type "make" or "gmake" inside a project directory. You must have GNU Make,
GCC 7+, NASM, Python 3 and GRUB2.

Final image works as a CD.

## How to use
Project contains Bochs config designed to work in any supported UNIX-like OS.
You could also run it in QEMU, or any another virtual (or real) machine…

### Real machine:
Write *live.iso* to any CD or USB stick.
You'll need x86_64 class PC, just because this code will jump to 64b mode of CPU.
Why? Because. That's why :)

### Bochs:

    bochs -q

### QEMU

    qemu-system-x86_64 -cdrom live.iso

### VirtualBox/VMWare/Other virtual machine

Add *live.iso* as a CD.