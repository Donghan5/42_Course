/* Exam no norm */

#include <unistd.h>

int main(int ac, char **av)
{
	int i;
	int flag;
	if (ac == 2)
	{
		i = 0;
		// while the av[1][i] is exist
		while (av[1][i])
		{
			// if the element is space or the tab
			if (av[1][i] == ' ' || av[1][i] == '\t')
			// set flag to 1
				flag = 1;
			// if the element is the word(form of the character)
			if (!(av[1][i] == ' ' || av[1][i] == '\t'))
			{
				// before there is a white space or tab space
				// replace tab or multiple spaces to single space
				if (flag)
					write(1, " ", 1);
				flag = 0; // set flag to 0
				write(1, &av[1][i], 1);
			}
			i += 1;
		}
	}
	write(1, "\n", 1);
	return (0);
}
