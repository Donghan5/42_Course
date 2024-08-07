/* During the exam there is no norminette rules */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_write_num(int n)
{
	if (n >= 10)
		ft_write_num(n / 10);
	ft_putchar("0123456789"[n % 10]);
}

int main(void)
{
	int	i = 1;
	while (i <= 100)
	{
		if ((i % 3) == 0)
			write(1, "fizz", 4);
		else if ((i % 5) == 0)
			write(1, "buzz", 4);
		else if ((i % 3) == 0 && (i % 5) == 0)
			write(1, "fizzbuzz", 8);
		else
			ft_write_num(i);
		ft_putchar('\n');
		i++;
	}
	return (0);
}

/* clarify the condition like if else if else */
