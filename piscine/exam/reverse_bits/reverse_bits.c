unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char	res;
	int	i;

	i = 8;
	while (i--)
	{
		res = res * 2 + (octet % 2);
		octet /= 2;
	}
	return (res);
}
