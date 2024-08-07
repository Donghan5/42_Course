/* During the exam there is not norm rules */

#include <unistd.h>

void ulstr(char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 32;
			write(1, &str[i], 1);
		}
		else if (str[i] >= 'A')
		{
			str[i] += 32;
			write(1, &str[i], 1);
		}
		else
			write(1, &str[i], 1);
		i++;
	}
}

int main(int ac, char **av)
{
	if (ac == 2)
		ulstr(av[1]);
	write(1, "\n", 1);
	return (0);
}

/* to change lowercase to uppercase, uppercase to lowercase */
