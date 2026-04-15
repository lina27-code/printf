#include "main.h"

int _printf(const char *format, ...)
{
    va_list args;
    int count, i;
    fmt_t fmt;
    const char *p;  /* moved to top */

    count = 0;
    i = 0;

    if (format == NULL)
        return (-1);

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '\0')
                break;

            p = &format[i];
            parse_format(&p, args, &fmt);
            i = p - format;

            switch (fmt.spec)
            {
            case 'c':
                count += print_char(args, &fmt);
                break;
            case 's':
                count += print_string(args, &fmt);
                break;
            case '%':
                count += print_percent(args, &fmt);
                break;
            case 'd':
            case 'i':
                count += print_int(args, &fmt);
                break;
            case 'b':
                count += print_binary(args, &fmt);
                break;
            case 'u':
                count += print_unsigned(args, &fmt);
                break;
            case 'o':
                count += print_octal(args, &fmt);
                break;
            case 'x':
                count += print_hex_lower(args, &fmt);
                break;
            case 'X':
                count += print_hex_upper(args, &fmt);
                break;
            case 'S':
                count += print_S(args, &fmt);
                break;
            case 'p':
                count += print_pointer(args, &fmt);
                break;
            case 'r':
                count += print_rev(args, &fmt);
                break;
            case 'R':
                count += print_rot13(args, &fmt);
                break;
            default:
                buf_char('%');
                buf_char(fmt.spec);
                count += 2;
                break;
            }
        }
        else
        {
            buf_char(format[i]);
            count++;
        }
        i++;
    }

    flush_buffer();
    va_end(args);
    return count;
}
