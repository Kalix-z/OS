GLOBAL _start
EXTERN kmain
_start:
    ;reserves space for the stack  
    section .bss
    stackPtr:
    resb 256000 ;reserves 250k for stack
    section .text
    mov esp, stackPtr
    call kmain
    jmp $
