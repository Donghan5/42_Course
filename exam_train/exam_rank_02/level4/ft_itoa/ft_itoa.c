#include <stdlib.h>

/* to calculate the length of the number */
int ft_numlen(int n)
{
	int size = 0;

	if (n < 0)
		n *= -1;
	if (n == 0)
		return (1);
	while (n--)
	{
		n /= 10;
		size++;
	}
	return (size);
}

/* to check the number is negative */
int	is_negative(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

char	*ft_itoa(int nbr)
{
	char	*res;
	int	len = ft_numlen(nbr);
	int negative = is_negative(nbr);

	if (nbr == -2147483648)
		return ("-2147483648\0");
	res = (char *)malloc(sizeof(char) * (negative + len + 1));
	if (!res)
		return (NULL);
	if (nbr == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = -nbr;
	}
	/* consider of the length when the number is negative number */
	res[len + negative] = '\0';
	/* beware of the condition of while */
	while (len-- > 0)
	{
		res[len + negative] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (res);
}

/* more study the logic */
