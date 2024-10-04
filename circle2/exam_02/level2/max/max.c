/* exam no norm rule */
#include <unistd.h>

int max(int *tab, unsigned int len)
{
	int	max;

	// edge case the len is less and equal than 0 and tab is null
	if (len <= 0 || tab == NULL)
		return (0);
	max = tab[--len]; // don't forget to --len to set the last element of the index
	while(len--)
	{
		// if the element is bigger than max
		if (tab[len] > max)
			// make tab[len] to max
			max = tab[len];
	}
	return (max);
}

/* read and understand the logics */
