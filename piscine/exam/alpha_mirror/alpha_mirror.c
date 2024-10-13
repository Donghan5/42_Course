#include <unistd.h>

int	main(int argc, char **argv)
{
	int	i;
	if (argc == 2)
	{
		i = 0;
		while (argv[1][i] != '\0')
		{
			if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
			{
				argv[1][i] = 'z' - argv[1][i] + 'a';
				write(1, &argv[1][i], 1);
			}
			if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
			{
				argv[1][i] = 'Z' - argv[1][i] + 'A';
				write(1, &argv[1][i], 1);
			}
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}
