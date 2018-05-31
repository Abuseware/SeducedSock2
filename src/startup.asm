[cpu x64]
[bits 32]

extern kmain
global _start

%assign MB_MAGIC 0x1BADB002
%assign MB_FLAGS 1

%assign PAGETABLE_ADDR 0x1000

%assign GDT_TYPE_DPL0 (0 << 13) | (0 << 14)
%assign GDT_TYPE_DPL1 (1 << 13)
%assign GDT_TYPE_DPL2 (2 << 13)
%assign GDT_TYPE_DPL3 (3 << 13)
%assign GDT_TYPE_P (1 << 15)
%assign GDT_TYPE_S (1 << 12)

%assign GDT_TYPE_DATA_RO (0 << 8)
%assign GDT_TYPE_DATA_ROA (1 << 8)
%assign GDT_TYPE_DATA_RW (2 << 8)
%assign GDT_TYPE_DATA_RWA (3 << 8)
%assign GDT_TYPE_DATA_RO_ED (4 << 8)
%assign GDT_TYPE_DATA_ROA_ED (5 << 8)
%assign GDT_TYPE_DATA_RW_ED (6 << 8)
%assign GDT_TYPE_DATA_RWA_ED (7 << 8)

%assign GDT_TYPE_CODE_X (8 << 8)
%assign GDT_TYPE_CODE_XA (9 << 8)
%assign GDT_TYPE_CODE_RX (10 << 8)
%assign GDT_TYPE_CODE_RXA (11 << 8)
%assign GDT_TYPE_CODE_XC (12 << 8)
%assign GDT_TYPE_CODE_XCA (13 << 8)
%assign GDT_TYPE_CODE_RXC (14 << 8)
%assign GDT_TYPE_CODE_RXCA (15 << 8)

%assign GDT_FLAG_L (1 << 5)
%assign GDT_FLAG_AVL (1 << 4)
%assign GDT_FLAG_DB (1 << 6)
%assign GDT_FLAG_G (1 << 7)

%assign PT_P (1 << 0)
%assign PT_RW (1 << 1)
%assign PT_US (1 << 2)
%assign PT_PWT (1 << 3)
%assign PT_PCD (1 << 4)
%assign PT_PS (1 << 7)
%assign PT_XD (1 << 63)

struc gdtr
  .size:  resw 1
  .base:  resd 1
endstruc

struc gdt_entry
  .limit: resw 1
  .base:  resw 1
  .type:      resw 1
  .flags:     resw 1
endstruc

[segment .multiboot]
dd MB_MAGIC
dd MB_FLAGS
dd -(MB_MAGIC + MB_FLAGS)


[segment .text]
_start:
  cli

  ;;Prepare paging
  xor eax, eax
  mov edi, PAGETABLE_ADDR
  mov ecx, 4096
  rep stosd

  mov DWORD [PAGETABLE_ADDR], PT_P | PT_RW | (PAGETABLE_ADDR + 0x1000)
  mov DWORD [PAGETABLE_ADDR + 0x1000], PT_P | PT_RW | (PAGETABLE_ADDR + 0x2000)
  mov DWORD [PAGETABLE_ADDR + 0x2000], PT_P | PT_RW | PT_PS

  ;; Prepare for long mode
  mov eax, PAGETABLE_ADDR
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
  jmp $

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
      dq 0
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

    .size equ $ - .gdt

[segment .bss]
resb 0x10000
stack:
