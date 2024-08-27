/* Exam no norm rule */
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	snake_to_camel(char *str)
{
	int	idx = 0;

	// when str is not null
	while (str[idx] != '\0')
	{
		// if index of the str is _
		if (str[idx] == '_')
		{
			// first increment of the idx
			idx++;
			// and add the space
			str[idx] -= ' ';
		}
		ft_putchar(str[idx]);
		idx++;
	}
}

int	main(int argc, char **argv)
{
	if (argc == 2)
		snake_to_camel(argv[1]);
	write(1, "\n", 1);
	return (0);
}
