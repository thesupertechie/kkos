#include <stdint.h>

void printf(const char* str) {
    volatile uint16_t* video_memory = (uint16_t*) 0xB8000;
    uint16_t attribute = 0x0F00; // white on black
    int offset = 0;
    
    while (*str) {
        video_memory[offset] = (*str) | attribute;
        str++;
        offset++;
    }
}

extern void main() {
    printf("kkOS i386 build");
}