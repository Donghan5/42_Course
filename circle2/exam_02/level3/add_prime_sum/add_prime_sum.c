/* exam no norm */
#include <unistd.h>

/* in this atoi, the argument is only number, so just convert
because of the prime number is always positive */
int	ft_atoi(char *num)
{
	int	res = 0;
	while (*num)
		res = res * 10 + *num++ - 48;
	return (res);
}

/* by initializing index 2(2 is minimum prime number)
check number is less or equal than 1 (to return 0)
for the condition while loop using square of i (to search faster)
if num % i == 0 it means that is not a prime number (so return 0)*/
int	is_prime(int num)
{
	int i = 2;

	if (num <= 1)
		return (0);
	while (i * i <= num)
	{
		if (num % i == 0)
			return (0);
		i++;
	}
	return (1);
}

/* this is num to print in write function cast convert */
void	ft_putnbr(int num)
{
	if (num >= 10)
		ft_putnbr(num / 10);
	char digit = num % 10 + '0';
	write(1, &digit, 1);
}

/* this scope num is ft_atoi(av[1]) and decrement of the num
verify the num is prime num and check if num is prime number
add sum (sum is like store by adding all prime numbers)*/
int main(int ac, char **av)
{
	if (ac == 2)
	{
		int nbr = ft_atoi(av[1]);
		int sum = 0;
		while (nbr > 0)
		{
			if (is_prime(nbr))
				sum += nbr;
			nbr--;
		}
		ft_putnbr(sum);
	}
	if (ac != 2)
		ft_putnbr(0);
	write(1, "\n", 1);
	return (0);
}
