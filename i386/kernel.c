#include <stdint.h>

typedef struct
{
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t attributes;
    uint16_t offset_2;
} __attribute__((packed)) idt32;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr32;

__attribute__((aligned(0x10)))
static idt32 idt[256];
static idtr32 idtr;

void set_idt_entry(int num, uint32_t base, uint16_t selector, uint8_t attributes){
    idt[num].offset_1 = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].attributes = attributes;
    idt[num].offset_2 = (base >> 16) & 0xFFFF;
}

void load_idt(uint32_t idtr_address){
    __asm__ __volatile__ ("lidt (%0)" : : "r" (idtr_address));
}

void isr_handler (int interrupt) {
    if (interrupt < 32){
        *(char*)0xb8000 = 'E';
         __asm__ __volatile__ ("cli");
        __asm__ __volatile__ ("hlt");
    }
    else {
        *(char*)0xb8000 = 'I';
    }

    __asm__ __volatile__ ("iret");
}

void isr0() {isr_handler(0);}
void isr1() {isr_handler(1);}
void isr2() {isr_handler(2);}
void isr3() {isr_handler(3);}
void isr4() {isr_handler(4);}
void isr5() {isr_handler(5);}
void isr6() {isr_handler(6);}
void isr7() {isr_handler(7);}
void isr8() {isr_handler(8);}
void isr9() {isr_handler(9);}
void isr10() {isr_handler(10);}
void isr11() {isr_handler(11);}
void isr12() {isr_handler(12);}
void isr13() {isr_handler(13);}
void isr14() {isr_handler(14);}
void isr15() {isr_handler(15);}
void isr16() {isr_handler(16);}
void isr17() {isr_handler(17);}
void isr18() {isr_handler(18);}
void isr19() {isr_handler(19);}
void isr20() {isr_handler(20);}
void isr21() {isr_handler(21);}
void isr22() {isr_handler(22);}
void isr23() {isr_handler(23);}
void isr24() {isr_handler(24);}
void isr25() {isr_handler(25);}
void isr26() {isr_handler(26);}
void isr27() {isr_handler(27);}
void isr28() {isr_handler(28);}
void isr29() {isr_handler(29);}
void isr30() {isr_handler(30);}
void isr31() {isr_handler(31);}

void generic_isr(){isr_handler(32);}

void idt_init(){
    for (int i=0; i<256; i++){
        set_idt_entry(i, (uint32_t)generic_isr, 0x08, 0x8E);
    }

    set_idt_entry(0, (uint32_t)isr0, 0x08, 0x8E);
    set_idt_entry(1, (uint32_t)isr1, 0x08, 0x8E);
    set_idt_entry(2, (uint32_t)isr2, 0x08, 0x8E);
    set_idt_entry(3, (uint32_t)isr3, 0x08, 0x8E);
    set_idt_entry(4, (uint32_t)isr4, 0x08, 0x8E);
    set_idt_entry(5, (uint32_t)isr5, 0x08, 0x8E);
    set_idt_entry(6, (uint32_t)isr6, 0x08, 0x8E);
    set_idt_entry(7, (uint32_t)isr7, 0x08, 0x8E);
    set_idt_entry(8, (uint32_t)isr8, 0x08, 0x8E);
    set_idt_entry(9, (uint32_t)isr9, 0x08, 0x8E);
    set_idt_entry(10, (uint32_t)isr10, 0x08, 0x8E);
    set_idt_entry(11, (uint32_t)isr11, 0x08, 0x8E);
    set_idt_entry(12, (uint32_t)isr12, 0x08, 0x8E);
    set_idt_entry(13, (uint32_t)isr13, 0x08, 0x8E);
    set_idt_entry(14, (uint32_t)isr14, 0x08, 0x8E);
    set_idt_entry(15, (uint32_t)isr15, 0x08, 0x8E);
    set_idt_entry(16, (uint32_t)isr16, 0x08, 0x8E);
    set_idt_entry(17, (uint32_t)isr17, 0x08, 0x8E);
    set_idt_entry(18, (uint32_t)isr18, 0x08, 0x8E);
    set_idt_entry(19, (uint32_t)isr19, 0x08, 0x8E);
    set_idt_entry(20, (uint32_t)isr20, 0x08, 0x8E);
    set_idt_entry(21, (uint32_t)isr21, 0x08, 0x8E);
    set_idt_entry(22, (uint32_t)isr22, 0x08, 0x8E);
    set_idt_entry(23, (uint32_t)isr23, 0x08, 0x8E);
    set_idt_entry(24, (uint32_t)isr24, 0x08, 0x8E);
    set_idt_entry(25, (uint32_t)isr25, 0x08, 0x8E);
    set_idt_entry(26, (uint32_t)isr26, 0x08, 0x8E);
    set_idt_entry(27, (uint32_t)isr27, 0x08, 0x8E);
    set_idt_entry(28, (uint32_t)isr28, 0x08, 0x8E);
    set_idt_entry(29, (uint32_t)isr29, 0x08, 0x8E);
    set_idt_entry(30, (uint32_t)isr30, 0x08, 0x8E);
    set_idt_entry(31, (uint32_t)isr31, 0x08, 0x8E);

    idtr.limit = (sizeof(idt32) * 256) - 1;
    idtr.base = (uint32_t)&idt;
    load_idt((uint32_t)&idtr);
}

extern void main(){
    idt_init();
    *(char*)0xb8000 = 'K';
    return;
}