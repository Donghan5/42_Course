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

/* I.E. imagine the octet is 38(in base 10)
first of all bit is 0 so 0 + 0 (why? the 38 % 2 is 0)
and after 38 / 2 is 19, so 0 + 1 (why? the 19 % 2 is 1)
decrement of the idx loop like this */
