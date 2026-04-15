#include "main.h"
#include <stdlib.h>

/* Helper: print signed number with precision */
int print_number_with_fmt(long n, fmt_t *fmt)
{
    unsigned long num;
    int count, digits, leading_zeros, i;
    char sign_char;
    char buffer[32];

    count = 0;
    digits = 0;
    leading_zeros = 0;
    sign_char = 0;

    if (n < 0)
    {
        sign_char = '-';
        num = -n;
    }
    else
    {
        if (fmt->plus)
            sign_char = '+';
        else if (fmt->space)
            sign_char = ' ';
        num = n;
    }

    /* count digits */
    if (num == 0)
        digits = 1;
    else
    {
        unsigned long tmp = num;
        while (tmp)
        {
            digits++;
            tmp /= 10;
        }
    }

    /* precision */
    if (fmt->precision > digits)
        leading_zeros = fmt->precision - digits;
    else if (fmt->precision == 0 && num == 0)
        digits = 0;

    /* print sign */
    if (sign_char)
    {
        buf_char(sign_char);
        count++;
    }

    /* leading zeros */
    for (i = 0; i < leading_zeros; i++)
    {
        buf_char('0');
        count++;
    }

    /* digits */
    if (digits > 0)
    {
        i = 0;
        while (num > 0)
        {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
        if (i == 0)
            buffer[i++] = '0';
        while (i > 0)
        {
            buf_char(buffer[--i]);
            count++;
        }
    }
    return count;
}

/* %d and %i */
int print_int(va_list args, fmt_t *fmt)
{
    int n = va_arg(args, int);
    return print_number_with_fmt(n, fmt);
}

/* %c */
int print_char(va_list args, fmt_t *fmt)
{
    char c;
    (void)fmt;
    c = (char)va_arg(args, int);
    buf_char(c);
    return 1;
}

/* %s (with precision) */
int print_string(va_list args, fmt_t *fmt)
{
    char *str;
    int len;
    str = va_arg(args, char *);
    if (str == NULL)
        str = "(null)";

    if (fmt->precision >= 0)
    {
        len = 0;
        while (len < fmt->precision && str[len])
            len++;
    }
    else
    {
        len = 0;
        while (str[len])
            len++;
    }
    buf_write(str, len);
    return len;
}

/* %% */
int print_percent(va_list args, fmt_t *fmt)
{
    (void)args;
    (void)fmt;
    buf_char('%');
    return 1;
}

/* %b (binary) */
int print_binary(va_list args, fmt_t *fmt)
{
    unsigned int n;
    char buffer[33];
    int i, count;
    (void)fmt;

    n = va_arg(args, unsigned int);
    i = 0;
    count = 0;

    if (n == 0)
    {
        buf_char('0');
        return 1;
    }

    while (n > 0)
    {
        buffer[i++] = (n % 2) + '0';
        n /= 2;
    }
    while (i > 0)
    {
        buf_char(buffer[--i]);
        count++;
    }
    return count;
}

/* helper for unsigned numbers with precision */
static int print_unsigned_base(unsigned long n, fmt_t *fmt, int base, int uppercase)
{
    char digits[] = "0123456789abcdef";
    char buffer[64];
    int i, count, num_digits, leading_zeros, j;
    i = 0;
    count = 0;
    num_digits = 0;
    leading_zeros = 0;

    /* count digits */
    if (n == 0)
        num_digits = 1;
    else
    {
        unsigned long tmp = n;
        while (tmp)
        {
            num_digits++;
            tmp /= base;
        }
    }

    if (fmt->precision > num_digits)
        leading_zeros = fmt->precision - num_digits;
    else if (fmt->precision == 0 && n == 0)
        num_digits = 0;

    /* convert digits */
    if (num_digits > 0)
    {
        unsigned long tmp = n;
        while (tmp)
        {
            int rem = tmp % base;
            char c = digits[rem];
            if (uppercase && rem > 9)
                c = c - 32;
            buffer[i++] = c;
            tmp /= base;
        }
        if (i == 0)
            buffer[i++] = '0';
    }

    /* leading zeros */
    for (j = 0; j < leading_zeros; j++)
    {
        buf_char('0');
        count++;
    }

    /* digits in reverse order */
    while (i > 0)
    {
        buf_char(buffer[--i]);
        count++;
    }
    return count;
}

/* %u */
int print_unsigned(va_list args, fmt_t *fmt)
{
    unsigned int n = va_arg(args, unsigned int);
    return print_unsigned_base(n, fmt, 10, 0);
}

/* %o */
int print_octal(va_list args, fmt_t *fmt)
{
    unsigned int n = va_arg(args, unsigned int);
    return print_unsigned_base(n, fmt, 8, 0);
}

/* %x */
int print_hex_lower(va_list args, fmt_t *fmt)
{
    unsigned int n = va_arg(args, unsigned int);
    return print_unsigned_base(n, fmt, 16, 0);
}

/* %X */
int print_hex_upper(va_list args, fmt_t *fmt)
{
    unsigned int n = va_arg(args, unsigned int);
    return print_unsigned_base(n, fmt, 16, 1);
}

/* %S (non‑printable chars as \xHH) */
int print_S(va_list args, fmt_t *fmt)
{
    char *str;
    int count;
    (void)fmt;

    str = va_arg(args, char *);
    count = 0;
    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        unsigned char c = *str;
        if (c >= 32 && c < 127)
        {
            buf_char(c);
            count++;
        }
        else
        {
            char hex[5];
            hex[0] = '\\';
            hex[1] = 'x';
            hex[2] = "0123456789ABCDEF"[(c >> 4) & 0xF];
            hex[3] = "0123456789ABCDEF"[c & 0xF];
            hex[4] = '\0';
            buf_write(hex, 4);
            count += 4;
        }
        str++;
    }
    return count;
}

/* %p (pointer) */
int print_pointer(va_list args, fmt_t *fmt)
{
    void *ptr;
    unsigned long addr;
    int count, i;
    char buffer[20];
    (void)fmt;

    ptr = va_arg(args, void *);
    if (ptr == NULL)
    {
        buf_write("(nil)", 5);
        return 5;
    }

    addr = (unsigned long)ptr;
    buf_write("0x", 2);
    count = 2;

    i = 0;
    if (addr == 0)
    {
        buf_char('0');
        count++;
    }
    else
    {
        while (addr > 0)
        {
            int rem = addr % 16;
            buffer[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
            addr /= 16;
        }
        while (i > 0)
        {
            buf_char(buffer[--i]);
            count++;
        }
    }
    return count;
}

/* %r (reverse string) */
int print_rev(va_list args, fmt_t *fmt)
{
    char *str;
    int len, i;
    (void)fmt;

    str = va_arg(args, char *);
    if (str == NULL)
        str = "(null)";
    len = 0;
    while (str[len])
        len++;
    for (i = len - 1; i >= 0; i--)
        buf_char(str[i]);
    return len;
}

/* %R (rot13) */
int print_rot13(va_list args, fmt_t *fmt)
{
    char *str;
    int count;
    (void)fmt;

    str = va_arg(args, char *);
    count = 0;
    if (str == NULL)
        str = "(null)";

    while (*str)
    {
        char c = *str;
        if (c >= 'a' && c <= 'z')
            c = ((c - 'a' + 13) % 26) + 'a';
        else if (c >= 'A' && c <= 'Z')
            c = ((c - 'A' + 13) % 26) + 'A';
        buf_char(c);
        count++;
        str++;
    }
    return count;
}
