/* During the exam, there is not norminette rules */
#include <unistd.h>

int	ft_isspace(char i)
{
	if (i == '\t' || i == '\n' || i == '\v' || i == '\f' || i == ' ')
		return (1);
	return (0);
}

void	ft_first_word(char *str)
{
	while (ft_isspace(*str))
		str++;
	while (*str && !(ft_isspace(*str)))
		write(1, str++, 1);
}

int	main(int argc, char *argv[])
{
	if (argc == 2)
		ft_first_word(argv[1]);
	write(1, "\n", 1);
	return (0);
}
