#include "vga.h"
#include <stdint.h>

#define VIDEO_MEMORY 0xB8000

void VGA_print_char(uint8_t color, char c, int offset) {
    uint16_t* video_memory = (uint16_t*) VIDEO_MEMORY;
    video_memory[offset] = (color << 8) | c;
    // shift color left so it can be a 16-bit value and add char to the end
};

void VGA_print_str(uint8_t color, char* str, int offset){
    int string_offset = offset;

    while (*str){
        VGA_print_char(color, *str, string_offset);
        str++;
        string_offset++;
    }
};