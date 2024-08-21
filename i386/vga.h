#include <stdint.h>

#ifndef VGA_H_
#define VGA_H_

void VGA_print_char(uint8_t color, char c, int offset);

void VGA_print_str(uint8_t color, char* str, int offset);

#endif