
section .multiboot_header
    ALA equ     0
    MEMINFO equ 0
    VIDINFO equ 0
    MAGIC_NUMBER equ 0x1BADB002                        ; define the magic number constant
    FLAGS        equ ALA | MEMINFO |VIDINFO            ; multiboot flags
    CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)           ; calculate the checksum
                                                       ; (magic number + checksum + flags should equal 0)
multiboot_header:
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum
    ;padding (probably used for something we dont need)
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
