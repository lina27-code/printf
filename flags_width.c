#include "main.h"

/**
 * parse_format - parses flags, width, precision, length modifiers
 * @format: pointer to current position in format string (after '%')
 * @args: va_list for reading '*' arguments
 * @fmt: structure to fill
 * Return: number of characters consumed (excluding the specifier itself)
 */
int parse_format(const char **format, va_list args, fmt_t *fmt)
{
    const char *p = *format;

    /* Initialize fmt */
    fmt->plus = 0;
    fmt->space = 0;
    fmt->hash = 0;
    fmt->zero = 0;
    fmt->minus = 0;
    fmt->width = 0;
    fmt->precision = -1;
    fmt->length = 0;
    fmt->spec = 0;

    /* Parse flags */
    while (*p == '+' || *p == ' ' || *p == '#' || *p == '0' || *p == '-')
    {
        if (*p == '+') fmt->plus = 1;
        if (*p == ' ') fmt->space = 1;
        if (*p == '#') fmt->hash = 1;
        if (*p == '0') fmt->zero = 1;
        if (*p == '-') fmt->minus = 1;
        p++;
    }

    /* Parse width */
    if (*p == '*')
    {
        fmt->width = va_arg(args, int);
        if (fmt->width < 0)
        {
            fmt->minus = 1;
            fmt->width = -fmt->width;
        }
        p++;
    }
    else
    {
        while (*p >= '0' && *p <= '9')
        {
            fmt->width = fmt->width * 10 + (*p - '0');
            p++;
        }
    }

    /* Parse precision */
    if (*p == '.')
    {
        p++;
        if (*p == '*')
        {
            fmt->precision = va_arg(args, int);
            if (fmt->precision < 0)
                fmt->precision = -1;
            p++;
        }
        else
        {
            fmt->precision = 0;
            while (*p >= '0' && *p <= '9')
            {
                fmt->precision = fmt->precision * 10 + (*p - '0');
                p++;
            }
        }
    }

    /* Parse length modifiers */
    if (*p == 'l')
    {
        fmt->length = 'l';
        p++;
    }
    else if (*p == 'h')
    {
        fmt->length = 'h';
        p++;
    }

    /* The specifier character */
    fmt->spec = *p;
    p++;

    *format = p;
    return (p - *format);
 }
