/* exam no norm rule */
#include <stdlib.h>

/* this is a reverse range so start at the end */
int	*ft_rrange(int start, int end)
{
	int	*range;
	int i = 0;
	int n = end - start;

	if (n < 0)
		n *= -1;
	n++; // to count all element of the array
	range = (int *)malloc(sizeof(int) * n);
	if (range)
	{
		while (i < n)
		{
			range[i] = end;
			end--;
			i++;
		}
	}
	return (range);
}
