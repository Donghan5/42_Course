/* Exam no norm */

#include <unistd.h>

int main(int ac, char **av)
{
	int i;
	int flag;
	if (ac == 2)
	{
		i = 0;
		while (av[1][i])
		{
			// if the element is space or the tab
			if (av[1][i] == ' ' || av[1][i] == '\t')
			// set flag to 1
				flag = 1;
			if (!(av[1][i] == ' ' || av[1][i] == '\t'))
			{
				// before there is a white space or tab space
				if (flag)
					write(1, " ", 1);
				flag = 0;
				write(1, &av[1][i], 1);
			}
			i += 1;
		}
	}
	write(1, "\n", 1);
	return (0);
}
