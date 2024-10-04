/* During the exam sesson no norm rules */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	do_op(char *str1, char operation, char *str2)
{
	int op1 = atoi(str1);
	int op2 = atoi(str2);
	int	res = 0;
	// separate all the case of opeartor like + - * /
	if (operation == '+')
		res = op1 + op2;
	else if (operation == '-')
		res = op1 - op2;
	else if (operation == '*')
		res = op1 * op2;
	else if (operation == '/')
		res = op1 / op2;
	printf("%d", res);
}

int main(int ac, char **av)
{
	if (ac == 4)
	{
		// make sure of the just one operator in the second arguement
		if (av[2][1] == '\0')
			do_op(av[1], av[2][0], av[3]);
	}
	printf("\n");
	return (0);
}
