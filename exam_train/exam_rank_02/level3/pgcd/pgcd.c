/* Exam no norm */
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	int num1;
	int num2;
	if (ac == 3)
	{
		if ((num1 = atoi(av[1])) > 0 && (num2 = atoi(av[2])) > 0)
		{
			while (num1 != num2)
			{
				if (num1 > num2)
					num1 -= num2;
				else
					num2 -= num1;
			}
			printf("%d", num1);
		}
		printf("\n");
	}
	return (0);
}