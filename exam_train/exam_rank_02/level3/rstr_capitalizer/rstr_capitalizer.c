/* Exam no norm rule */
#include <unistd.h>

// lower to upper the last word (if lower) to upper
void	rstr_capitalizer(char *str)
{
	int i = 0;

	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z') // conver lower to upper
			str[i] += 32;
		// to verify next index of the last letter of the word is a tab space or '\0
		if ((str[i] >= 'a' && str[i] <= 'z') && (str[i + 1] == ' ' || str[i + 1] == '\t' || str[i + 1] == '\0'))
			str[i] -= 32;
		write(1, &str[i], 1);
		i++;
	}
}

int main(int ac, char **av)
{
	int i;

	if (ac == 1)
		write(1, "\n", 1);
	else
	{
		i = 1;
		while (i < ac)
		{
			rstr_capitalizer(av[i]);
			write(1, "\n", 1); // end of print new-line char
			i++;
		}
	}
	return (0);
}
