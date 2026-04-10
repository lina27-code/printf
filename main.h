#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>

/* main function */
int _printf(const char *format, ...);

/* helper functions */
int _putchar(char c);
int print_number(int n);
int print_binary(unsigned int n);

#endif
