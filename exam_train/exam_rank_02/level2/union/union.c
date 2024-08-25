/* Exam no norm rule */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	check_ascii_tab(int *ascii_tab, char c)
{
	if (ascii_tab[(int)c] == 0)
	{
		ft_putchar(c);
		ascii_tab[(int)c] = 1;
	}
}

void	ft_union(char *str1, char *str2)
{
	int i;
	int	ascii_tab[256];

	i = 0;
	while (i < 256)
	{
		ascii_tab[i] = 0;
		i++;
	}
	i = 0;
	while (str1[i] != '\0')
	{
		check_ascii_tab(ascii_tab, str1[i]);
		i++;
	}
	i = 0;
	while (str2[i] != '\0')
	{
		check_ascii_tab(ascii_tab, str2[i]);
		i++;
	}
}

int main(int argc, char **argv)
{
	if (argc == 3)
		ft_union(argv[1], argv[2]);
	ft_putchar('\n');
	return (0);
}

/* have to review the logics the case of 1st test */
