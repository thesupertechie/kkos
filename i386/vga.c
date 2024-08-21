#include "vga.h"
#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define outb(port, data) __asm__ volatile ("outb %0, %1" :: "a"(data), "Nd"(port))
uint8_t screen_color;

void VGA_set_colors(uint8_t new_color){
    screen_color = new_color;
}

void VGA_print_char(char c, int offset) {
    uint16_t* video_memory = (uint16_t*) VIDEO_MEMORY;
    video_memory[offset] = (screen_color << 8) | c;
    // shift color left so it can be a 16-bit value and add char to the end
};

void VGA_print_str(char* str, int offset){
    int string_offset = offset;

    while (*str){
        VGA_print_char(*str, string_offset);
        str++;
        string_offset++;
    }
};