#include "main.h"

/**
 * print_pointer - prints pointer address
 * @ptr: pointer
 * Return: count
 */
int print_pointer(void *ptr)
{
	unsigned long addr;
	int count = 0;

	if (!ptr)
		return (_putchar('(') + _putchar('n') + _putchar('i') +
		        _putchar('l') + _putchar(')'));

	addr = (unsigned long)ptr;

	count += _putchar('0');
	count += _putchar('x');

	count += print_base(addr, 16, 0);

	return (count);
}
