/* exam no norm rule */
#include <unistd.h>

int max(int *tab, unsigned int len)
{
	int	max;

	if (len <= 0 || tab == NULL)
		return (0);
	max = tab[--len];
	while(len--)
	{
		if (tab[len] > max)
			max = tab[len];
	}
	return (max);
}

/* read and understand the logics */
