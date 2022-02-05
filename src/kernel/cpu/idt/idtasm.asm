[GLOBAL idt_flush]    ; Allows the Cpp code to call idt_flush().

idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]        ; Load the IDT pointer.
   sti               ; reenables interupts
   ret
  