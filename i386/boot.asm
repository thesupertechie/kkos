[bits 16]
[org 0x7c00]

VIDEO_MEMORY equ 0xb8000
KERNEL_LOCATION equ 0x1000

; Read from first two sectors of disk
mov [BOOTDISK], dl ; set bootdisk to disk number
xor ax, ax ; zero out ax
mov es, ax ; set extra segment to 0
mov ds, ax ; set data segment to 0
mov bp, 0x8000 ; set stack area
mov sp, bp ; initialize stack
mov bx, KERNEL_LOCATION ; set bx to kernel location
mov dh, 2 ; read 2 sectors
mov ah, 0x02 ; tell BIOS we are going to read from disk
mov al, dh ; again, 2 sectors
mov ch, 0x00 ; cylinder 0
mov dh, 0x00 ; head 0
mov cl, 0x02 ; set starting sector to 2
mov dl, [BOOTDISK] ; load the boot disk number
int 0x13 ; tell BIOS to read disk

mov ah, 0x0 ; tell BIOS we are going to switch to TTY mode
mov al, 0x3 ; 80x25 text mode
int 0x10 ; tell BIOS to do this

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode
jmp $

BOOTDISK: db 0

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code:
        dw 0xFFFF   ; Limit (4 GiB)
        dw 0x0      ; Base (low 16 bits)
        db 0x0      ; Base (next 8 bits)
        db 0x9A     ; Access byte: code segment, ring 0, executable, readable
        db 0xCF     ; Granularity: 4KiB pages, 32-bit segment
        db 0x0      ; Base (high 8 bits)
    gdt_data:
        dw 0xFFFF   ; Limit (4 GiB)
        dw 0x0      ; Base (low 16 bits)
        db 0x0      ; Base (next 8 bits)
        db 0x92     ; Access byte: data segment, ring 0, writable
        db 0xCF     ; Granularity: 4KiB pages, 32-bit segment
        db 0x0      ; Base (high 8 bits)
gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]
start_protected_mode:
    ; Load segment registers with new data segment
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_LOCATION
    jmp $

times 510-($-$$) db 0
dw 0xaa55
