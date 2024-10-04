/* exam no norm rule */
#include <unistd.h>

// to convert param to integer
int	ft_atoi(char *s)
{
	int i = 0;
	int res = 0;
	int sign = 1;
	while (s[i] == '\n' || s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\v')
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		i++;
	}
	return (sign * res);
}

void	print_hex(int n)
{
	char hex_digit[] = "0123456789abcdef";
	if (n > 16)
		print_hex(n / 16);
	write(1, &hex_digit[n % 16], 1);
}

int main(int ac, char **av)
{
	if (ac == 2)
		print_hex(ft_atoi(av[1]));
	write(1, "\n", 1);
	return (0);
}
