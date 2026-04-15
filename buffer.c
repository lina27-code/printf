#include <unistd.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];
static int buf_idx = 0;

/**
 * flush_buffer - writes the buffer content to stdout and resets it
 */
void flush_buffer(void)
{
    if (buf_idx > 0)
        write(1, buffer, buf_idx);
    buf_idx = 0;
}

/**
 * buf_write - writes a string to the buffer
 * @str: string to write
 * @len: number of bytes to write
 */
void buf_write(const char *str, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (buf_idx >= BUFFER_SIZE)
            flush_buffer();
        buffer[buf_idx++] = str[i];
    }
}

/**
 * buf_char - writes a single character to the buffer
 * @c: character to write
 */
void buf_char(char c)
{
    if (buf_idx >= BUFFER_SIZE)
        flush_buffer();
    buffer[buf_idx++] = c;
}
