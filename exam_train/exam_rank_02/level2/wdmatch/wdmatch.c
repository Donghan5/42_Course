/* Exam no norm rule */
#include <unistd.h>

int	ft_strlen(char *str)
{
	int size = 0;

	while (str[size] != '\0')
		size++;
	return (size);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	wdmatch(char *first, char *second)
{
	int i = 0;
	int	j = 0;

	while (second[j] != '\0')
	{
		if (first[i] == second[j])
			i++;
		j++;
	}
	if (i == ft_strlen(first))
		ft_putstr(first);
}

int main(int argc, char **argv)
{
	if (argc == 3)
		wdmatch(argv[1], argv[2]);
	write(1, "\n", 1);
	return (0);
}
