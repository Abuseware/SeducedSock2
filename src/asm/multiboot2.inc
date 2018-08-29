%assign MULTIBOOT_SEARCH 32768
%assign MULTIBOOT_HEADER_ALIGN 8

%assign MULTIBOOT2_HEADER_MAGIC 0xe85250d6
%assign MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

%assign MULTIBOOT_MOD_ALIGN 0x00001000
%assign MULTIBOOT_INFO_ALIGN 0x00000008

%assign MULTIBOOT_TAG_ALIGN 8
%assign MULTIBOOT_TAG_TYPE_END 0
%assign MULTIBOOT_TAG_TYPE_CMDLINE 1
%assign MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME 2
%assign MULTIBOOT_TAG_TYPE_MODULE 3
%assign MULTIBOOT_TAG_TYPE_BASIC_MEMINFO 4
%assign MULTIBOOT_TAG_TYPE_BOOTDEV 5
%assign MULTIBOOT_TAG_TYPE_MMAP 6
%assign MULTIBOOT_TAG_TYPE_VBE 7
%assign MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8
%assign MULTIBOOT_TAG_TYPE_ELF_SECTIONS 9
%assign MULTIBOOT_TAG_TYPE_APM 10
%assign MULTIBOOT_TAG_TYPE_EFI32 11
%assign MULTIBOOT_TAG_TYPE_EFI64 12
%assign MULTIBOOT_TAG_TYPE_SMBIOS 13
%assign MULTIBOOT_TAG_TYPE_ACPI_OLD 14
%assign MULTIBOOT_TAG_TYPE_ACPI_NEW 15
%assign MULTIBOOT_TAG_TYPE_NETWORK 16
%assign MULTIBOOT_TAG_TYPE_EFI_MMAP 17
%assign MULTIBOOT_TAG_TYPE_EFI_BS 18
%assign MULTIBOOT_TAG_TYPE_EFI32_IH 19
%assign MULTIBOOT_TAG_TYPE_EFI64_IH 20
%assign MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR 21

%assign MULTIBOOT_HEADER_TAG_END 0
%assign MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST 1
%assign MULTIBOOT_HEADER_TAG_ADDRESS 2
%assign MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS 3
%assign MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS 4
%assign MULTIBOOT_HEADER_TAG_FRAMEBUFFER 5
%assign MULTIBOOT_HEADER_TAG_MODULE_ALIGN 6
%assign MULTIBOOT_HEADER_TAG_EFI_BS 7
%assign MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32 8
%assign MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64 9
%assign MULTIBOOT_HEADER_TAG_RELOCATABLE 10

%assign MULTIBOOT_ARCHITECTURE_I386 0
%assign MULTIBOOT_ARCHITECTURE_MIPS32 4
%assign MULTIBOOT_HEADER_TAG_OPTIONAL 1

%assign MULTIBOOT_LOAD_PREFERENCE_NONE 0
%assign MULTIBOOT_LOAD_PREFERENCE_LOW 1
%assign MULTIBOOT_LOAD_PREFERENCE_HIGH 2

%assign MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
%assign MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2

%assign MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
%assign MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
%assign MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2

struc multiboot_header
  .magic: resd 1
  .arch: resd 1
  .length: resd 1
  .checksum: resd 1
endstruc

struc multiboot_header_tag
  .type: resw 1
  .flags: resw 1
  .size: resd 1
endstruc

struc multiboot_header_tag_information_request
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .requests: resd 1
endstruc

struc multiboot_header_tag_address
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .header_addr: resd 1
  .load_addr: resd 1
  .load_end_addr: resd 1
  .bss_end_addr: resd 1
endstruc

struc multiboot_header_tag_entry_address
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .entry_addr: resd 1
endstruc

struc multiboot_header_tag_console_flags
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .console_flags: resd 1
endstruc

struc multiboot_header_tag_framebuffer
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .width: resd 1
  .height: resd 1
  .depth: resd 1
endstruc

struc multiboot_header_mtag_module_align
  .type: resw 1
  .flags: resw 1
  .size: resd 1
endstruc

struc multiboot_header_tag_relocatable
  .type: resw 1
  .flags: resw 1
  .size: resd 1
  .min_addr: resd 1
  .max_addr: resd 1
  .align: resd 1
  .preference: resd 1
endstruc
