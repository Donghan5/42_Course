#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

int	main(int argc, char **argv)
{
	int	i;
	int	len;
	if (argc == 2)
	{
		len = ft_strlen(argv[1]);
		i = len;
		while (i--)
		{
			if ((argv[1][i] >= 9 && argv[1][i] <= 13) || argv[1][i] == ' ')
				break ;
		}
		while (argv[1][++i] != '\0')
			write(1, &argv[1][i], 1);
	}
	write(1, "\n", 1);
	return (0);
}

