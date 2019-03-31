[bits 32]

%include "gdt.inc"
%include "pagetable.inc"

extern kmain
global _start
global mb_info

[segment .text]
_start:
  cli

  test eax, 0x2BADB002
  jnz multiboot_ok
  mov dx, 0x64
  mov al, 0xfe
  out dx, al

multiboot_ok:
  mov [mb_info], ebx

  ; Set PML4
  mov edx, PDP
  or edx, PT_P | PT_RW
  mov DWORD [PML4], edx ; (PML4E points to PDPTEs - 1GiB pages)

  ; Set PDP
  ;mov edx, PD
  ;or edx, PT_P | PT_RW
  ;mov DWORD [PDP], edx ; (PDPTEs points to PDEs - 2MiB pages)

  ; Set PD
  ;mov edx, PT
  ;or edx, PT_P | PT_RW
  ;mov DWORD [PD], edx ; (PDPTEs points to PTEs - 4KiB pages)

  ;Fill Page entries
  mov ecx, 0
  fill:
    mov eax, ecx
    shl eax, 30
    or eax, PT_P | PT_RW | PT_PS
    mov [PDP + (ecx * 8)], eax
    mov eax, ecx
    shr eax, 2
    mov [PDP + (ecx * 8) + 4], eax
    inc ecx
    cmp ecx, 512
    jne fill

  ;; Prepare for long mode
  mov eax, PML4
  mov cr3, eax

  ;; Set PAE
  mov eax, cr4
  or eax, (1 << 5)
  mov cr4, eax

  ;; Set EFER
  mov ecx, 0xC0000080
  rdmsr
  or eax, (1 << 8)
  wrmsr

  ;; Set paging (PG)
  mov eax, cr0
  or eax, (1 << 31)
  mov cr0, eax

  ;; Load GDT for long mode
  lgdt [gdt64.gdtr]

  jmp DWORD 0x8:mode64

[bits 64]
mode64:

  mov rsp, stack
  mov rbp, rsp

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov fs, ax
  mov gs, ax

  call kmain

  mov rsp, rbp

  ; Reset after kernel exit
  mov dx, 0x64
  mov al, 0xfe
  out dx, al

[segment .data]
align 4
gdt64:
  .gdtr:
  istruc gdtr
  at gdtr.size, dw .size
  at gdtr.base, dd .gdt
  iend

  .gdt:
    .gdt_null:
      ;; Null segment
      istruc gdt_entry
        at gdt_entry.limit, dw 0xffff
        at gdt_entry.base, dw 0
        at gdt_entry.type, dw 0
        at gdt_entry.flags, dw GDT_FLAG_G | GDT_FLAG_L
      iend
    .gdt_code:
      ;; Code segment
      istruc gdt_entry
        at gdt_entry.limit, dw 0xffff
        at gdt_entry.base, dw 0
        at gdt_entry.type, dw GDT_TYPE_P | GDT_TYPE_S | GDT_TYPE_CODE_RX
        at gdt_entry.flags, dw 0xf | GDT_FLAG_G | GDT_FLAG_L | GDT_FLAG_AVL
      iend

    .gdt_data:
    ;; Data
      istruc gdt_entry
        at gdt_entry.limit, dw 0xffff
        at gdt_entry.base, dw 0
        at gdt_entry.type, dw GDT_TYPE_P | GDT_TYPE_S | GDT_TYPE_DATA_RW
        at gdt_entry.flags, dw 0xf | GDT_FLAG_G | GDT_FLAG_L | GDT_FLAG_AVL
      iend

    .size equ $ - .gdt - 1

[segment .bss]
resb 0x10000
stack:

align 4096
PML4: resq 512
align 4096
PDP: resq 512
align 4096
PD: resq 512
align 4096
PT: resq 512


mb_info:
resd 1
