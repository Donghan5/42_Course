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

int main(int ac, char **av)
{
	char *str;
	char *f_word_start;
	char *f_word_end;

	if (ac > 1)
	{
		str = av[1];
		// pass the space and tab to search start of the word
		while (*str == ' ' || *str == '\t')
			str++;
		f_word_start = str;
		// pass the character to search end of the word
		while (*str != ' ' && *str != '\t' && *str != '\0')
			str++;
		f_word_end = str;
		// after the word pass space and tab
		while (*str == ' ' || *str == '\t')
			str++;
		if (*str)
		{
			while (*str)
			{
				// this one to multiple spaces or tab to single space
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
/*
	function flow

	searching the start of the first word
					|
	searching the end of the first word
					|
	pass the space or tab after the first word
					|
	printing the remain word(skip multiple spaces or tabs and print single spaces)
					|
	print the first word at the end (by using write_word)

	write_word it take two parameters which are f_word_start and f_word_end
					|
	untill start < end write(1, start, 1) to print the character
*/
