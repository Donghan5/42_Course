/* Exam no norm */
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int	i = 0;
		int	start;
		int	end;

		while (av[1][i] != '\0')
			i++;
		while (i >= 0)
		{
			// searching the starting point of the end
			while (av[1][i] == ' ' || av[1][i] == '\t' || av[1][i] == '\0')
				i--;
			end = i;
			while (av[1][i] && av[1][i] != ' ' && av[1][i] != '\t')
				i--;
			start = i + 1;
			int flag = start;
			while (start <= end)
			{
				write(1, &av[1][start], 1);
				start++;
			}
			if (flag != 0)
				write(1, " ", 1);
		}
	}
	write(1, "\n", 1);
	return (0);
}
