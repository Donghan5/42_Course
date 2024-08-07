/* During the exam there is not norminette rules */
#include <unistd.h>

int main(int ac, char **av)
{
	if(ac == 2)
	{
		int i = 0;
		while (av[1][i] != '\0')
			i++;
		while (i--)
			write(1, &av[1][i], 1);
	}
	write(1, "\n", 1);
	return (0);
}

/* idea : increasing index of the argv and decrease with write func */
