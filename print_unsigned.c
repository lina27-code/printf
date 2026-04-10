#include "main.h"

/**
 * print_unsigned - prints unsigned integer
 * @n: number
 * Return: count
 */
int print_unsigned(unsigned int n)
{
	int count = 0;

	if (n / 10)
		count += print_unsigned(n / 10);

	count += _putchar((n % 10) + '0');

	return (count);
}
