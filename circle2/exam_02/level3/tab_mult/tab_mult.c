/* exam no norm */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

// tuyrn to interger type in the parameter
int ft_atoi(char *str)
{
	int sign = 1;
	int res = 0;

// while (*str == ' ' || (*str >= 9 && &str <= 13)) --> more shorter
	while (*str == ' ' || *str == '\t' || *str == '\v' || *str == '\n' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (sign * res);
}

void	ft_putnbr(int num)
{
	if (num >= 10)
		ft_putnbr(num / 10);
	ft_putchar(num % 10 + '0');
}

int	main(int ac, char **av)
{
	int i; // index of the 1, 2, 3, 4 .. in the subject
	int	nbr;

	if (ac == 2)
	{
		i = 1;
		nbr = ft_atoi(av[1]);
		while (i <= 9 && nbr <= 2147483647)
		{
			ft_putnbr(i);
			write(1, " x ", 3);
			ft_putnbr(nbr);
			write(1, " = ", 3);
			ft_putnbr(i * nbr);
			write(1, "\n", 1);
			i += 1;
		}
	}
	else
		write(1, "\n", 1);
	return (0);
}
