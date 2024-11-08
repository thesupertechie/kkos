[org 0x7c00]
KERNEL_LOCATION equ 0x1000
mov [BOOT_DISK], dl

xor ax, ax
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov bx, KERNEL_LOCATION
mov dh, 2

mov ah, 0x02
mov al, dh
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK] ; disk 0
int 0x13 ; TODO: Implement error checking

mov ah, 0x0
mov al, 0x3
int 0x10 ; clear text buffer

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $

BOOT_DISK: db 0

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0

    gdt_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0x9A
        db 0xCF
        db 0x0
    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0x92
        db 0xCF
        db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov fs, ax
        mov gs, ax
        mov ebp, 0x90000
        mov esp, ebp
    jmp KERNEL_LOCATION

times 510-($-$$) db 0
dw 0xaa55