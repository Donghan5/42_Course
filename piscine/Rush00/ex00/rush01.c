#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	rush(int x, int y)
{
	int	i;
	int	j;
	
	j = 1;
	while (j <= y)
	{
		i = 1;
		while(i <= x)
		{
			if((j == 1 && i == 1) || (j == y && i == x && x > 1 && y > 1))
				ft_putchar('/');
			else if((j == 1 && i == x) || (j == y && i == 1))
				ft_putchar('\\');
			else if((j == 1 || j == y) || (i == 1 || i == x))
				ft_putchar('*');
			else
				ft_putchar(' ');
			i++;
		}
		ft_putchar('\n');
		j++;
	}
}

int main()
{
	rush(5, 3);
	ft_putchar('\n');
	rush(5, 1);
	ft_putchar('\n');
	rush(1, 1);
	ft_putchar('\n');
	rush(1, 5);
	ft_putchar('\n');
	rush(4, 4);
	ft_putchar('\n');
	return(0);
}
