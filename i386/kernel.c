#define IDT_MAX_DESCRIPTORS 256

#include <stdint.h>
#include <stdbool.h>
#include "vga.h"

__attribute__((noreturn))
void exception_handler(uint8_t interrupt_number);

extern void exception_handler(uint8_t interrupt_number){
    __asm__ volatile ("cli");
    __asm__ volatile ("hlt");
}

void panic(char* error_code){
    VGA_set_colors(0x4F);
    for (int i = 0; i < (80*25); i++) {
        VGA_print_char(' ', i);
    }

    VGA_print_str("STOP:", 0);

    int error_code_offset = 5;

    while (*error_code){
        VGA_print_char(*error_code, (error_code_offset));
        error_code++;
        error_code_offset++;
    }

    VGA_print_str("An issue occurred which required kkOS to stop. If this is the first time you", 80);
    VGA_print_str("have seen this message, please restart your computer. If not, please open a new", 160);
    VGA_print_str("issue at kkOS's GitHub repository:", 240);
    VGA_print_str("https://github.com/kkCoder111/kkos", 320);
    VGA_print_str("Please ensure that no similar issue exists, and include a screenshot.", 400);
    VGA_print_str("Thank you.", 480);
}

typedef struct {
    uint16_t offset_lo;
    uint16_t kernel_selector;
    uint8_t unused;
    uint8_t type_attributes;
    uint16_t offset_hi;
} __attribute__ ((packed)) idt_entry;

struct idtr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

// Declare the IDT array
idt_entry idt[IDT_MAX_DESCRIPTORS];

// Declare the IDTR structure
struct idtr idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];

void set_idt_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry* descriptor = &idt[vector];
    descriptor->offset_lo = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_selector = 0x08;
    descriptor->offset_hi = (uint32_t)isr >> 16;
    descriptor->unused = 0;
    descriptor->type_attributes = flags;
}

void idt_init(void){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++){
        set_idt_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" :: "m"(idtr));
    __asm__ volatile ("sti");
}

extern void main(){
    enum VGA_colors {
        BLACK = 0x0,
        BLUE = 0x1,
        GREEN = 0x2,
        CYAN = 0x3,
        RED = 0x4,
        MAGENTA = 0x5,
        BROWN = 0x6,
        LIGHT_GRAY = 0x7,
        DARK_GRAY = 0x8,
        LIGHT_BLUE = 0x9,
        LIGHT_GREEN = 0xA,
        LIGHT_CYAN = 0xB,
        LIGHT_RED = 0xC,
        LIGHT_MAGENTA = 0xD,
        YELLOW = 0xE,
        WHITE = 0xF
    };
    VGA_set_colors(0x0F);
    VGA_print_str("kkOS 32-bit", 0);
    VGA_print_str("The architecture type is i386.", 80);
    //int a = 1 / 0; // This will trigger a divide by zero interrupt
    return;
}
