/* Exam no norm rule */
#include <unistd.h>

/* first word to capitalizer */
void	str_capitalizer(char *str)
{
	int i = 0;

	// the case of one letter (i.e. "a First")
	if (str[i] >= 'a' && str[i] <= 'z')
		str[i] -= 32;
	write(1, &str[i], 1);
	while (str[++i]) // prefix increment operator
	{
		// in the middle of the word if uppercase --> turn to lowercase
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		// the start of the word if lowercase --> turn to uppercase
		if ((str[i] >= 'a' && str[i] <= 'z') && (str[i - 1] == ' ' || str[i - 1] == '\t'))
			str[i] -= 32;
		write(1, &str[i], 1);
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
			str_capitalizer(av[i]);
			write(1, "\n", 1);
			i++;
		}
	}
	return (0);
}

/* 로직 다시 한 번만 더 공부하기 */
