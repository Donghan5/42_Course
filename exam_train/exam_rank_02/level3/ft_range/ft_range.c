/* exam no norm */
#include <stdlib.h>

int	*ft_range(int start, int end)
{
	int	*range;
	int	len;
	int	i = 0;

	if (end - start >= 0)
		len = (end - start) + 1;
	if (end - start < 0)
		len = ((end - start) * -1) + 1;
	range = (int *)malloc(sizeof(int) * len);
	while (i < len)
	{
		if (start < end)
		{
			range[i] = start;
			start++;
			i++;
		}
		else
		{
			range[i] = end;
			end--;
			i++;
		}
	}
	return (range);
}
