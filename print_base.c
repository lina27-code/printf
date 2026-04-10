#include "main.h"

/**
 * print_base - prints number in any base
 * @n: number
 * @base: base
 * @is_upper: uppercase flag
 * Return: count
 */
int print_base(unsigned int n, int base, int is_upper)
{
	char *digits;
	int count = 0;

	if (is_upper)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";

	if (n / base)
		count += print_base(n / base, base, is_upper);

	count += _putchar(digits[n % base]);

	return (count);
}
