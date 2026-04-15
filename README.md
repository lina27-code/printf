# Custom Printf – WinMingle Project

## Description

This project is a custom implementation of the standard C library function `printf`.  
It is written from scratch using only allowed system calls and functions: `write`, `malloc`, `free`, and the `va_*` macros (`va_start`, `va_arg`, `va_end`, `va_copy`).  

The implementation follows the **WinMingle** project requirements and is compiled on **Ubuntu 20.04 LTS** with the flags:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89

The primary goals are to understand:
- Variadic functions (`va_list`, `va_start`, `va_arg`, `va_end`)
- Formatted output handling
- Low‑level I/O with `write`
- Internal buffering to minimise system calls
- Parsing of flags, width, precision, and length modifiers

---

## Features Supported

### Conversion Specifiers

| Specifier | Description                                      | Example                     | Output                    |
|-----------|--------------------------------------------------|-----------------------------|---------------------------|
| `%c`      | Prints a single character                        | `_printf("%c", 'A');`       | `A`                       |
| `%s`      | Prints a string (NULL → `(nil)`)                | `_printf("%s", NULL);`      | `(nil)`                   |
| `%%`      | Prints a literal percent sign                   | `_printf("%%");`            | `%`                       |
| `%d`, `%i`| Prints a signed decimal integer                 | `_printf("%d", -123);`      | `-123`                    |
| `%b`      | Prints an unsigned int in binary                | `_printf("%b", 98);`        | `1100010`                 |
| `%u`      | Prints an unsigned decimal integer              | `_printf("%u", 123);`       | `123`                     |
| `%o`      | Prints an unsigned octal integer                | `_printf("%o", 8);`         | `10`                      |
| `%x`      | Prints unsigned hexadecimal (lowercase)         | `_printf("%x", 255);`       | `ff`                      |
| `%X`      | Prints unsigned hexadecimal (uppercase)         | `_printf("%X", 255);`       | `FF`                      |
| `%S`      | Prints a string, replacing non‑printable chars with `\xHH` | `_printf("%S", "Best\nSchool");` | `Best\x0ASchool` |
| `%p`      | Prints a pointer address (NULL → `(nil)`)       | `_printf("%p", ptr);`       | `0x7ffe637541f0` or `(nil)` |
| `%r`      | Prints the reversed string                      | `_printf("%r", "hello");`   | `olleh`                   |
| `%R`      | Prints the rot13'ed string                      | `_printf("%R", "hello");`   | `uryyb`                   |

### Flags (for non‑custom specifiers)

| Flag | Description                                          | Example               | Output      |
|------|------------------------------------------------------|-----------------------|-------------|
| `+`  | Always print a sign (+ or -)                        | `_printf("%+d", 42);` | `+42`       |
| space| Print a space before positive numbers               | `_printf("% d", 42);` | ` 42`       |
| `#`  | Alternate form: `0` for octal, `0x`/`0X` for hex    | `_printf("%#x", 255);`| `0xff`      |
| `0`  | Pad with zeros (overridden by precision)            | `_printf("%05d", 42);`| `00042`     |
| `-`  | Left‑justify within the field width                 | `_printf("%-5d", 42);`| `42   `     |

### Field Width & Precision

- **Width**: Minimum number of characters to print.  
  Example: `_printf("%5d", 42);` → `"   42"`
- **Precision**:  
  - For integers: minimum number of digits (pads with zeros).  
  - For strings: maximum number of characters to print.  
  Example: `_printf("%.5d", 42);` → `"00042"`  
  Example: `_printf("%.3s", "hello");` → `"hel"`

Width and precision can be specified as `*` to read the value from the arguments:  
`_printf("%*.*d", 5, 3, 42);` → `"  042"`

### Length Modifiers

| Modifier | Effect                                 | Example (long)                |
|----------|----------------------------------------|-------------------------------|
| `l`      | Converts to `long` for `d`, `i`, `u`, `o`, `x`, `X` | `_printf("%ld", 123456789L);` |
| `h`      | Converts to `short` for `d`, `i`, `u`, `o`, `x`, `X` | `_printf("%hd", (short)32767);` |

---

## Compilation & Usage

### Compilation (without a test `main`)

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o libprintf.a
Code Structure
File	Responsibility
main.h	Header with prototypes, fmt_t structure, and include guards
_printf.c	Main loop: parses format string, calls parse_format, dispatches to handlers
flags_width.c	Parses flags, width, precision, length modifiers
print_handlers.c	Implementation of all conversion specifiers (c, s, d, i, b, u, o, x, X, S, p, r, R)
buffer.c	1024‑byte internal buffer to minimise write calls
_putchar.c	Simple write‑based character output (fallback)
README.md	This file
Edge Cases Handled
NULL format string → returns -1

NULL string argument → prints (nil) for %s, (nil) for %p

Empty format string → prints nothing, returns 0

Incomplete specifier (% at end of string) → prints nothing

Precision 0 with value 0 → prints nothing (for integers)

Negative width → treated as left‑justify flag

Negative precision → ignored (treated as not specified)

Zero flag ignored when precision is specified for integers

Hash flag (#) for %o, %x, %X adds prefix (0, 0x, 0X) even when value is 0? (Standard: for 0 with #, prints 0 – we follow standard.)

Limitations (compared to standard printf)
No support for %f, %e, %g (floating point)

No %n (write number of characters written)

No %m (print error message)

No locale‑specific formatting

No dynamic width/precision from * in all cases? Actually we support * for both width and precision.

Buffer is not thread‑safe (static buffer)

Testing
We strongly recommend testing with the following cases (compare output with standard printf):

c
/* Basic specifiers */
_printf("Hello %s, you are %d years old.\n", "Alice", 25);
_printf("Character: %c\n", 'A');
_printf("Percent: %%\n");

/* Numbers */
_printf("Positive: %d\n", 123);
_printf("Negative: %i\n", -456);
_printf("Zero: %d\n", 0);

/* Binary */
_printf("Binary of 98: %b\n", 98);

/* Unsigned, octal, hex */
_printf("Unsigned: %u\n", 4294967295U);
_printf("Octal: %o\n", 255);
_printf("Hex lower: %x\n", 255);
_printf("Hex upper: %X\n", 255);

/* Pointer */
int x = 42;
_printf("Pointer: %p\n", &x);
_printf("NULL pointer: %p\n", NULL);

/* String with non‑printable */
_printf("Special: %S\n", "Best\nSchool");

/* Flags */
_printf("Plus: %+d\n", 42);
_printf("Space: % d\n", 42);
_printf("Hash octal: %#o\n", 255);
_printf("Hash hex: %#x\n", 255);

/* Width and precision */
_printf("Width 5: |%5d|\n", 42);
_printf("Precision 5: |%.5d|\n", 42);
_printf("Width + precision: |%8.5d|\n", 42);
_printf("String precision: |%.3s|\n", "hello");

/* Length modifiers */
long l = 123456789L;
_printf("Long: %ld\n", l);
short s = 32767;
_printf("Short: %hd\n", s);

/* Reverse and rot13 */
_printf("Reversed: %r\n", "hello");
_printf("Rot13: %R\n", "hello");
Acknowledgements
Inspired by the standard C library printf.

Developed as part of the WinMingle Software Engineering Program.

Special thanks to the ALX low‑level programming curriculum.

License
This project is for educational purposes only.
Do not publish or plagiarise.
See the WinMingle academic integrity policy.
