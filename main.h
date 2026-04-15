#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>

/* buffer functions */
void flush_buffer(void);
void buf_write(const char *str, int len);
void buf_char(char c);

/* format structure */
typedef struct fmt_s
{
    int plus;        /* '+' flag */
    int space;       /* ' ' flag */
    int hash;        /* '#' flag */
    int zero;        /* '0' flag */
    int minus;       /* '-' flag */
    int width;       /* field width */
    int precision;   /* precision ( -1 if not specified ) */
    char length;     /* 'l' or 'h' */
    char spec;       /* conversion specifier */
} fmt_t;

/* parsing function (defined in flags_width.c) */
int parse_format(const char **format, va_list args, fmt_t *fmt);

/* handlers (all take va_list and fmt_t*) */
int print_char(va_list args, fmt_t *fmt);
int print_string(va_list args, fmt_t *fmt);
int print_percent(va_list args, fmt_t *fmt);
int print_int(va_list args, fmt_t *fmt);
int print_binary(va_list args, fmt_t *fmt);
int print_unsigned(va_list args, fmt_t *fmt);
int print_octal(va_list args, fmt_t *fmt);
int print_hex_lower(va_list args, fmt_t *fmt);
int print_hex_upper(va_list args, fmt_t *fmt);
int print_S(va_list args, fmt_t *fmt);
int print_pointer(va_list args, fmt_t *fmt);
int print_rev(va_list args, fmt_t *fmt);
int print_rot13(va_list args, fmt_t *fmt);

/* helper for number printing */
int print_number_with_fmt(long n, fmt_t *fmt);
int print_unsigned_with_fmt(unsigned long n, fmt_t *fmt, int base, int uppercase);

/* core printf function */
int _printf(const char *format, ...);

#endif
