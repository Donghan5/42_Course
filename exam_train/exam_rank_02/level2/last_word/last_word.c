/* Exam no norm rule */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_strlen(char *str)
{
	int size = 0;

	while (str[size] != '\0')
		size++;
	return (size);
}

void	last_word(char *str)
{
	int i = ft_strlen(str) - 1;

	while (str[i] == ' ' || str[i] == '\t')
		i--;
	while (str[i] != '\0' && (str[i] != ' ' && str[i] != '\t'))
		i--;
	i++;
	while (str[i] != '\0' && (str[i] != ' ' && str[i] != '\t'))
	{
		ft_putchar(str[i]);
		i++;
	}
}

int main(int ac, char **av)
{
	if (ac == 2)
		last_word(av[1]);
	ft_putchar('\n');
	return (0);
}
