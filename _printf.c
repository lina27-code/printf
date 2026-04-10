#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: format string
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	int i = 0, count = 0;
	va_list args;

	if (format == NULL)
		return (-1);

	va_start(args, format);

	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;

			if (format[i] == 'c')
				count += _putchar(va_arg(args, int));

			else if (format[i] == 's')
			{
				char *str = va_arg(args, char *);
				int j = 0;

				if (str == NULL)
					str = "(null)";

				while (str[j])
				{
					count += _putchar(str[j]);
					j++;
				}
			}

			else if (format[i] == 'd' || format[i] == 'i')
				count += print_number(va_arg(args, int));

			else if (format[i] == 'b')
				count += print_binary(va_arg(args, unsigned int));
           else if (format[i] == 'u')
	count += print_unsigned(va_arg(args, unsigned int));

else if (format[i] == 'o')
	count += print_base(va_arg(args, unsigned int), 8, 0);

else if (format[i] == 'x')
	count += print_base(va_arg(args, unsigned int), 16, 0);

else if (format[i] == 'X')
	count += print_base(va_arg(args, unsigned int), 16, 1);
else if (format[i] == 'S')
	count += print_string_special(va_arg(args, char *));

			else if (format[i] == '%')
				count += _putchar('%');

			else
			{
				count += _putchar('%');
				count += _putchar(format[i]);
			}
		}
		else
		{
			count += _putchar(format[i]);
		}

		i++;
	}

	va_end(args);
    _putchar(-1);

	return (count);
}
