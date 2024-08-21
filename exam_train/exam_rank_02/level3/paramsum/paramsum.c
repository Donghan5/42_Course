/* Exam no norm */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n >= 10)
		ft_putnbr(n / 10);
	ft_putchar(n % 10 + '0');
}

int main(int ac, char **av)
{
	(void)av;

	int num = ac - 1;
	ft_putnbr(num);
	return (0);
}
