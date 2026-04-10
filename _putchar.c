#include "main.h"

#define BUFFER_SIZE 1024

/**
 * _putchar - writes a character to buffer
 * @c: character
 * Return: 1
 */
int _putchar(char c)
{
	static char buffer[BUFFER_SIZE];
	static int index;

	if (c == -1 || index >= BUFFER_SIZE)
	{
		write(1, buffer, index);
		index = 0;
	}

	if (c != -1)
		buffer[index++] = c;

	return (1);
}
