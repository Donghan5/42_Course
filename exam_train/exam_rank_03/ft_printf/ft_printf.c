/* Exam no norm rule */
#include <unistd.h>
#include <stdarg.h>

// In this part using parameter char * and int * to get the address of the len
void	put_string(char *str, int *length)
{
	// str is empty and treat as real printf
	// declare str as (null)
	if (!str)
		str = "(null)";
	// when str, by using pointer operation with write func
	while (*str)
		*length += write(1, str++, 1);
}

// this one to put digit, parameter is long long int num, int base and int *length
void	put_digit(long long int number, int base, int *length)
{
	// when is the number is negative number number multiple by -1 and calculate pointer operation with write(1, "-", 1)
	if (number < 0)
	{
		number *= -1;
		*length += write(1, "-", 1);
	}
	// if the number is bigger than base, recursive, and number / base
	if (number >= base)
		put_digit((number / base), base, length);
	// if not, modulus with write (in base 16 chars)
	*length += write(1, &("0123456789abcdef"[number % base]), 1);
}

int	ft_printf(const char *format, ... )
{
	int length = 0;

	// to using va_arg declare as pointer
	va_list	pointer;
	// don't forget ;
	va_start(pointer, format);
	while (*format)
	{
		if ((*format == "%" && ((*(format + 1) == 's') || (*(format + 1) == 'd') || (*format + 1) == 'x')))
		{
			// first of all increment of the format
			format++;
			if (*format == 's')
				put_string(va_arg(pointer, char *), &length);
			else if (*format == 'd')
				put_digit((long long int)va_arg(pointer, int), 10, &length);
			else if (*format == 'x')
				put_digit((long long int)va_arg(pointer, unsigned int), 16, &length);
		}
		else
			length += write(1, format, 1);
		format++;
	}
	// to return (va_end(pointer), and length)
	return (va_end(pointer), length);
}
