[bits 16]
Program_Space equ 0x8000

DiskRead:
    mov ah, 0x02 ;tells the bios that when we call the interupt 0x13, we want to read from the disk
    mov bx, Program_Space ;tells the bios where the program is located (same as the orgin)
    mov al, 127;amount of sectors to read
    mov dl, [Boot_Disk] ;tells the bios which disk to read from, which is orginally stored in the Dl register (passed this in Bootloader.asm / the main file) 
    mov ch, 0x00 ; cylinder to read fron (n/a for us)
    mov dh, 0x00 ; head to read (n/a for us)
    mov cl, 0x02 ;Which sector to read from next

    int 0x13

    jc DiskReadFailed
     
    ret

Boot_Disk:
    db 0

DiskReadFailed:
    mov ah, 0x0e
    mov al, 'D'
    int 0x10
    mov al, 'i'
    int 0x10
    mov al, 's'
    int 0x10
    mov al, 'k'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'R'
    int 0x10
    mov al, 'e'
    int 0x10
    mov al, 'a'
    int 0x10
    mov al, 'd'
    int 0x10
    jmp $ 

