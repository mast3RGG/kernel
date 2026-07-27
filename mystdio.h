#ifndef MYSTDIO_H
#define MYSTDIO_H

#include <stdarg.h>
#include "types.h"

void vga_print_char(char c);
void vga_print_string(char *string);
void clean_screen(void);
void kprintf(const char *format, ...);

#endif
