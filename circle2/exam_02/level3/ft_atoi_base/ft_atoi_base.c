/* No norm */
#include <unistd.h>

/* this one is to convert to lower case (the demande of the subject)*/
char	to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}
/* c is verify character and digits_in_base (i.e. 2 or 10 maybe 8 or 16)*/
int	get_digit(char c, int digits_in_base)
{
	int	max_digit;

	// in base is less and equal than 10 it means using just number
	if (digits_in_base <= 10)
		// (i.e. digits_in_base is 10, so max_digit is 9 +'0' is to conver char type)
		max_digit = digits_in_base - 1 + '0';
	// consider of the range of the alphabetic number in 16-base case.
	else
		max_digit = digits_in_base - 10 - 1 + '0';
	if (c >= '0' && c <= '9' && c <= max_digit)
		return (c - '0');
	else if (c >= 'a' && c <= 'f' && c <= max_digit)
		return (10 + c - 'a');
	else
		return (-1);
}

int	ft_atoi_base(const char *str, int str_base)
{
	int	res = 0;
	int sign = 1;
	int digit;
	if (str == NULL || str_base < 2 || str_base > 16) // edge cases
		return (0);
	if (*str == '-') // consider minus cases
	{
		sign = -1;
		str++;
	}
	while (*str)
	{
		digit = get_digit(to_lower(*str), str_base);
		if (digit == -1)
			break ;
		res = res * str_base + digit;
		str++;
	}
	return (sign * res);
}
