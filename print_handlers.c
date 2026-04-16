#include "main.h"
#include <stdlib.h>

/* ---------- Helper: print signed number with width, precision, zero, minus ---------- */
int print_number_with_fmt(long n, fmt_t *fmt)
{
    unsigned long num;
    int count, digits, leading_zeros, i;
    char sign_char;
    char buffer[32];
    int total_len, pad_len;
    char pad_char;

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

    total_len = (sign_char ? 1 : 0) + leading_zeros + digits;

    if (fmt->zero && !fmt->minus && fmt->precision == -1)
        pad_char = '0';
    else
        pad_char = ' ';

    pad_len = (fmt->width > total_len) ? (fmt->width - total_len) : 0;

    /* right‑justified with spaces */
    if (!fmt->minus && pad_char == ' ')
    {
        for (i = 0; i < pad_len; i++)
            buf_char(' ');
        count += pad_len;
    }

    /* sign */
    if (sign_char)
    {
        buf_char(sign_char);
        count++;
    }

    /* zero padding (from zero flag) */
    if (!fmt->minus && pad_char == '0')
    {
        for (i = 0; i < pad_len; i++)
            buf_char('0');
        count += pad_len;
    }

    /* precision leading zeros */
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

    /* left‑justified trailing spaces */
    if (fmt->minus)
    {
        for (i = 0; i < pad_len; i++)
            buf_char(' ');
        count += pad_len;
    }

    return count;
}

/* ---------- Helper: print unsigned number (u, o, x, X) with hash, zero, width, precision, minus ---------- */
int print_unsigned_with_fmt(unsigned long n, fmt_t *fmt, int base, int uppercase)
{
    char digits[] = "0123456789abcdef";
    char buffer[64];
    int i, count, num_digits, leading_zeros, j;
    int total_len, pad_len;
    char pad_char;
    int prefix_len = 0;
    char prefix[3] = {0};

    count = 0;
    num_digits = 0;
    leading_zeros = 0;

    /* hash flag */
    if (fmt->hash && n != 0)
    {
        if (base == 8)
        {
            prefix[0] = '0';
            prefix_len = 1;
        }
        else if (base == 16)
        {
            prefix[0] = '0';
            prefix[1] = uppercase ? 'X' : 'x';
            prefix_len = 2;
        }
    }

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

    total_len = prefix_len + leading_zeros + num_digits;

    if (fmt->zero && !fmt->minus && fmt->precision == -1)
        pad_char = '0';
    else
        pad_char = ' ';

    pad_len = (fmt->width > total_len) ? (fmt->width - total_len) : 0;

    /* right‑justified spaces */
    if (!fmt->minus && pad_char == ' ')
    {
        for (j = 0; j < pad_len; j++)
            buf_char(' ');
        count += pad_len;
    }

    /* prefix */
    for (j = 0; j < prefix_len; j++)
    {
        buf_char(prefix[j]);
        count++;
    }

    /* zero padding from zero flag */
    if (!fmt->minus && pad_char == '0')
    {
        for (j = 0; j < pad_len; j++)
            buf_char('0');
        count += pad_len;
    }

    /* precision leading zeros */
    for (j = 0; j < leading_zeros; j++)
    {
        buf_char('0');
        count++;
    }

    /* convert digits */
    if (num_digits > 0)
    {
        unsigned long tmp = n;
        i = 0;
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
        while (i > 0)
        {
            buf_char(buffer[--i]);
            count++;
        }
    }

    /* left‑justified trailing spaces */
    if (fmt->minus)
    {
        for (j = 0; j < pad_len; j++)
            buf_char(' ');
        count += pad_len;
    }

    return count;
}

/* ---------- %d and %i ---------- */
int print_int(va_list args, fmt_t *fmt)
{
    long n;
    if (fmt->length == 'l')
        n = va_arg(args, long);
    else if (fmt->length == 'h')
        n = (short)va_arg(args, int);
    else
        n = va_arg(args, int);
    return print_number_with_fmt(n, fmt);
 }

/* ---------- %c (with width and minus) ---------- */
int print_char(va_list args, fmt_t *fmt)
{
    char c = (char)va_arg(args, int);
    int count = 0;
    int width = fmt->width;
    int left = fmt->minus;

    if (!left)
    {
        while (--width > 0)
        {
            buf_char(' ');
            count++;
        }
    }
    buf_char(c);
    count++;
    if (left)
    {
        while (--width > 0)
        {
            buf_char(' ');
            count++;
        }
    }
    return count;
}

/* ---------- %s (with width, precision, and minus) ---------- */
int print_string(va_list args, fmt_t *fmt)
{
    char *str = va_arg(args, char *);
    int len = 0;
    int count = 0;
    int width = fmt->width;
    int left = fmt->minus;
    int prec = fmt->precision;

    if (str == NULL)
        str = "(null)";

    if (prec >= 0)
    {
        while (len < prec && str[len])
            len++;
    }
    else
    {
        while (str[len])
            len++;
    }

    if (!left)
    {
        while (width > len)
        {
            buf_char(' ');
            count++;
            width--;
        }
    }
    buf_write(str, len);
    count += len;
    if (left)
    {
        while (width > len)
        {
            buf_char(' ');
            count++;
            width--;
        }
    }
    return count;
}
       

/* ---------- %% (with width and minus) ---------- */
 int print_percent(va_list args, fmt_t *fmt)
{
    int count;
    int width;
    int left;
    (void)args;

    count = 0;
    width = fmt->width;
    left = fmt->minus;

    if (!left)
    {
        while (--width > 0)
        {
            buf_char(' ');
            count++;
        }
    }
    buf_char('%');
    count++;
    if (left)
    {
        while (--width > 0)
        {
            buf_char(' ');
            count++;
        }
    }
    return count;
}
 

/* ---------- %b (binary) ---------- */
int print_binary(va_list args, fmt_t *fmt)
{
    unsigned int n;
    char buffer[33];
    int i;
    int count;
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

/* ---------- %u ---------- */
int print_unsigned(va_list args, fmt_t *fmt)
{
    unsigned long n;
    if (fmt->length == 'l')
        n = va_arg(args, unsigned long);
    else if (fmt->length == 'h')
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
    return print_unsigned_with_fmt(n, fmt, 10, 0);
}

/* ---------- %o ---------- */
int print_octal(va_list args, fmt_t *fmt)
{
    unsigned long n;
    if (fmt->length == 'l')
        n = va_arg(args, unsigned long);
    else if (fmt->length == 'h')
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
    return print_unsigned_with_fmt(n, fmt, 8, 0);
}

/* ---------- %x ---------- */
int print_hex_lower(va_list args, fmt_t *fmt)
{
    unsigned long n;
    if (fmt->length == 'l')
        n = va_arg(args, unsigned long);
    else if (fmt->length == 'h')
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
    return print_unsigned_with_fmt(n, fmt, 16, 0);
}

/* ---------- %X ---------- */
int print_hex_upper(va_list args, fmt_t *fmt)
{
    unsigned long n;
    if (fmt->length == 'l')
        n = va_arg(args, unsigned long);
    else if (fmt->length == 'h')
        n = (unsigned short)va_arg(args, unsigned int);
    else
        n = va_arg(args, unsigned int);
    return print_unsigned_with_fmt(n, fmt, 16, 1);
}

/* ---------- %S (non‑printable) ---------- */
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

/* ---------- %p (pointer) ---------- */
int print_pointer(va_list args, fmt_t *fmt)
{
    void *ptr;
    unsigned long addr;
    int count;
    int i;
    char buffer[20];
    (void)fmt;

    ptr = va_arg(args, void *);
    count = 0;
    i = 0;

    if (ptr == NULL)
    {
        buf_write("(nil)", 5);
        return 5;
    }
    addr = (unsigned long)ptr;
    buf_write("0x", 2);
    count = 2;
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

/* ---------- %r (reverse string) ---------- */
int print_rev(va_list args, fmt_t *fmt)
{
    char *str;
    int len;
    int i;
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

/* ---------- %R (rot13) ---------- */
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
