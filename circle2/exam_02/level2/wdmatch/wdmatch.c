/* Exam no norm rule */
#include <unistd.h>

int	ft_strlen(char *str)
{
	int size = 0;

	while (str[size] != '\0')
		size++;
	return (size);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	wdmatch(char *first, char *second)
{
	int i = 0;
	int	j = 0;

	// when the element of the second is not null
	while (second[j] != '\0')
	{
		// if the first element is equal to second element
		if (first[i] == second[j])
			// increment the index of the first
			i++;
		// if not increment the index of the second
		j++;
	}
	// if i is equal to length of the first
	if (i == ft_strlen(first))
		ft_putstr(first);
}

int main(int argc, char **argv)
{
	if (argc == 3)
		wdmatch(argv[1], argv[2]);
	write(1, "\n", 1);
	return (0);
}
