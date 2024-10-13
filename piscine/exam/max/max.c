#include <unistd.h>
#include <stdio.h>

int	max(int *tab, unsigned int len)
{
	int	max;

	max = tab[0];
	while (len--)
	{
		if (tab[len] > max)
			max = tab[len];
	}
	return (max);
}
