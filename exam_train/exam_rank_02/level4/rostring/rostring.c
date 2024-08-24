/* Exam no norm */
#include <unistd.h>
#include <stdlib.h>
void	write_word(char *start, char *end)
{
	while (start < end)
	{
		write(1, start, 1);
		start++;
	}
}

int	main(int ac, char **av)
{
	char	*str;
	char	*f_word_start;
	char	*f_word_end;

	if (ac > 1)
	{
		str = av[1];
		while (*str == ' ' || *str == '\t')
			str++;
		f_word_start = str;
		while (*str != ' ' || *str != '\t')
			str++;
		f_word_end = str;
		while (*str == ' ' || *str == '\t')
			str++;
		if (*str)
		{
			while (*str)
			{
				if (*str == ' ' || *str == '\t')
				{
					while (*str == ' ' || *str == '\t')
						str++;
					if (*str)
						write(1, " ", 1);
				}
				else
				{
					write(1, str, 1);
					str++;
				}
			}
			write(1, " ", 1);
		}
		write_word(f_word_start, f_word_end);
	}
	write(1, "\n", 1);
	return (0);
}
