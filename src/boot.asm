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
mov dl, [BOOT_DISK]
int 0x13 ; no error management!

mov ah, 0x0e
mov al, 'K'
int 0x10
mov al, 'B'
int 0x10
mov al, 'L'
int 0x10

mov ah, 0x0
mov al, 0x3
int 0x10

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli ; disable interrupts
lgdt [GDT_descriptor] ; self explanatory
mov eax, cr0 ; we need to change cr0 to tell it we're going into protected mode
or eax, 1 ; but we can't change it directly, so we need to copy it to another register and THEN change it
mov cr0, eax ; finally, changing the last bit to 1
jmp CODE_SEG:start_protected_mode

jmp $
BOOT_DISK: db 0

GDT_start:
    GDT_null: ; null segment
        dd 0x0
        dd 0x0
    GDT_code:
        dw 0xffff ; first 16 bits of limit (limit will be set to the highest value to ensure we can use all 4GB of memory)
        dw 0x0 ; first 24 bits of base
        db 0x0 ; base contains the address where the segment begins
        db 0b10011010 
        ; the instruction above defines present (1 for any valid segment),
        ; privilege (00-03, 00 is highest),
        ; type flag (1 means code/data),
        ; if the segment is code (1 for yes)
        ; conforming (can lower privilege applications call it) (1 for yes)
        ; readable (can i read constants) (1 for yes)
        ; accessed (managed by cpu) (1 for yes)
        db 0b11001111
        ; the instruction above defines the following:
        ; granularity
        ; 32 bits? (1 for yes)
        ; next two bits are used for 64 bit mode iirc
        ; end of limit
        db 0x0
    GDT_data:
        dw 0xffff ; first 16 bits of limit (limit will be set to the highest value to ensure we can use all 4GB of memory)
        dw 0x0 ; first 24 bits of base
        db 0x0 ; base contains the address where the segment begins
        db 0b10010010 
        ; the instruction above defines present (1 for any valid segment),
        ; privilege (00-03, 00 is highest),
        ; type flag (1 means code/data),
        ; if the segment is code (1 for yes)
        ; direction (1 for grows downwards)
        ; writable (1 for yes)
        ; accessed (managed by cpu) (1 for yes)
        db 0b11001111
        ; the instruction above defines the following:
        ; granularity
        ; 32 bits? (1 for yes)
        ; next two bits are used for 64 bit mode iirc
        ; end of limit
        db 0x0
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start
[bits 32]

start_protected_mode:
    ;mov al, '!'
    ;mov ah, 0x1f ; background foreground
    ;mov [0xb8000], ax
    ;jmp $
    ;setup segment registers
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION

; padding
times 510 - ($-$$) db 0
; add the magic number so the computer knows this is bootable
dw 0xaa55