/* during the exam no norm rule */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	alpha_mirror(char *str)
{
	int idx = 0;
	while (str[idx] != '\0')
	{
		if ((str[idx] >= 'A') && (str[idx] <= 'Z'))
			str[idx] = ('M' - (str[idx] - 'N'));
		else if ((str[idx] >= 'a' ) && (str[idx] <= 'z'))
			str[idx] = ('m' - str[idx] - 'n');
		ft_putchar(str[idx]);
		idx++;
	}
}

int main(int ac, char **av)
{
	if (ac == 2)
		alpha_mirror(av[1]);
	ft_putchar('\n');
	return (0);
}

/*
**	that is the formule to mirroring alphabetic
**	for example: 'M' - ('A' - 'N') = 'Z'
**	concerning the ascii table, M = 77, A = 65, N = 78, Z = 90
**	so, 77 - (65 - 78) = 77 - (-13) = 77 + 13 = 90
*/
