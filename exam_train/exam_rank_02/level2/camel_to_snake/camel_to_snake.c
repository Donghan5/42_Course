/* During the exam there is no norm rules */
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int i = 0;
		while (av[1][i] != '\0')
		{
			// when the element is uppercase
			if (av[1][i] >= 65 && av[1][i] <= 90)
			{
				// swap to lowercase
				av[1][i] += 32;
				// after put _
				write(1, "_", 1);
			}
			// after the if condition print
			write(1, &av[1][i], 1);
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}
