### OPTIONS
set disassembly-flavor intel
set architecture i386:x86-64:intel
set osabi none
set listsize 5
set pagination off

#commands
#begin
#  echo [ASM] ----------------------------------------------------------------\n
#  x/10i $rip
#  echo [REG] ----------------------------------------------------------------\n
#  info registers rax rbx rcx rdx rsi rdi rbp rsp r8 r9 r10 r11 r12 r13 r14 r15 rip eflags
#  echo [CALL] ---------------------------------------------------------------\n
#  backtrace
#  echo [ARGS] ---------------------------------------------------------------\n
#  info args
#  echo [VAR] ----------------------------------------------------------------\n
#  info local
#  echo ----------------------------------------------------------------------\n
#end

### Connect
file kernel
target extended-remote :1234

rbreak ^interrupt_*
