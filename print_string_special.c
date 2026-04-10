#include "main.h"

/**
 * print_string_special - prints string with special handling
 * @str: string
 * Return: count
 */
int print_string_special(char *str)
{
	int i = 0, count = 0;
	unsigned int c;

	if (!str)
		str = "(null)";

	while (str[i])
	{
		c = str[i];

		if (c < 32 || c >= 127)
		{
			count += _putchar('\\');
			count += _putchar('x');

			if (c < 16)
				count += _putchar('0');

			count += print_base(c, 16, 1);
		}
		else
		{
			count += _putchar(c);
		}
		i++;
	}

	return (count);
}
