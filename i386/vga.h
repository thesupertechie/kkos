#include <stdint.h>

#ifndef VGA_H_
#define VGA_H_

void VGA_set_colors(uint8_t new_color);

void VGA_print_char(char c, int offset);

void VGA_print_str(char* str, int offset);

#endif