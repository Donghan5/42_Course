/* Exam no norm rules */
#include <unistd.h>

unsigned char	reverse_bits(unsigned char octet)
{
	int		idx;
	unsigned char	bit = 0;

	idx = 7;
	while (idx >= 0)
	{
		bit = ((bit * 2) + (octet % 2));
		octet /= 2;
		idx--;
	}
	return (bit);
}
