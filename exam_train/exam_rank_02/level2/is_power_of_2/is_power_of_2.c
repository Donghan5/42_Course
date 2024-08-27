/* EXAM NO NORM RULE */

int	is_power_of_2(unsigned int n)
{
	// declare idx the type of the unsigned long int
	unsigned long int	idx = 1;
	// when idx is lower and equal than n
	while (idx <= n)
	{
		// if the idx is equal to n reutrn 1
		if (idx == n)
			return (1);
		// 2 times increment to verify power of 2 condition
		idx *= 2;
	}
	return (0);
}
