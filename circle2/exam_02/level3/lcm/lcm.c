/* Exam no norm */
#include <unistd.h>

/* lcm(최소 공배수) it means n modulus a and b is 0 return number */
unsigned int	lcm(unsigned int a, unsigned int b)
{
	unsigned int n;

	if (a == 0 || b == 0)
		return (0);
	// this is to set the starting point, bigger number is more efficient.
	if (a > b)
		n = a;
	else
		n = b;
	while (1)
	{
		if (n % a == 0 && n % b == 0)
			return (n);
		++n;
	}
}

/* have to study about lcm */
