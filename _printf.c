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
        int plus = 0, space = 0, hash = 0;
        int l = 0, h = 0;

			i++;
             
            /* Handle flags */
	while (format[i] == '+' || format[i] == ' ' || format[i] == '#')
	{
		if (format[i] == '+')
			plus = 1;
		else if (format[i] == ' ')
			space = 1;
		else if (format[i] == '#')
			hash = 1;

		i++;
	} 
         
          if (format[i] == 'l')
{
	l = 1;
	i++;
}
else if (format[i] == 'h')
{
	h = 1;
	i++;
}
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
{
	long int num;

	if (l)
		num = va_arg(args, long int);
	else if (h)
		num = (short int)va_arg(args, int);
	else
		num = va_arg(args, int);

	if (num >= 0)
	{
		if (plus)
			count += _putchar('+');
		else if (space)
			count += _putchar(' ');
	}

	count += print_number(num);
}	

			else if (format[i] == 'b')
				count += print_binary(va_arg(args, unsigned int));

    else if (format[i] == 'u')
	{
	unsigned long int num;

	if (l)
		num = va_arg(args, unsigned long int);
	else if (h)
		num = (unsigned short int)va_arg(args, unsigned int);
	else
		num = va_arg(args, unsigned int);

	count += print_unsigned(num);
}

else if (format[i] == 'o')
	{

	unsigned long int num;

	if (l)
		num = va_arg(args, unsigned long int);
	else if (h)
		num = (unsigned short int)va_arg(args, unsigned int);
	else
		num = va_arg(args, unsigned int);

	if (hash && num != 0)
		count += _putchar('0');

	count += print_base(num, 8, 0);
}

else if (format[i] == 'x')
	{

	unsigned long int num;

	if (l)
		num = va_arg(args, unsigned long int);
	else if (h)
		num = (unsigned short int)va_arg(args, unsigned int);
	else
		num = va_arg(args, unsigned int);

	if (hash && num != 0)
				{
		count += _putchar('0');
		count += _putchar('x');
				}

		count += print_base(num, 16, 0);
}

else if (format[i] == 'X')
{
	unsigned long int num;

	if (l)
		num = va_arg(args, unsigned long int);
	else if (h)
		num = (unsigned short int)va_arg(args, unsigned int);
	else
		num = va_arg(args, unsigned int);

	if (hash && num != 0)
{
	    count += _putchar('0');
		count += _putchar('X');
}
    count += print_base(num, 16, 1);
}	

else if (format[i] == 'S')
	count += print_string_special(va_arg(args, char *));
else if (format[i] == 'p')
	count += print_pointer(va_arg(args, void *));

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
