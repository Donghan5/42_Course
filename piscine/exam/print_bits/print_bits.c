#include <unistd.h>

void	print_bits(unsigned char octet)
{
	unsigned char	binary;
	int	i;

	i = 8;
	while (i--)
	{
		binary = (octet >> i & 1) + '0';
		write(1, &binary, 1);
	}
}
