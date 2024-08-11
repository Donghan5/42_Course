/* During the exam session there is not norm rules */

int	ft_atoi(char *str)
{
	int idx = 0;
	int	sign = 1;
	int	res = 0;

	while (str[idx] == ' ' || (str[idx] == '\t' && str[idx] == '\r'))
		idx++;
	if (str[idx] == '+' || str[idx] == '-')
	{
		if (str[idx] == '-')
			sign = -1;
		idx++;
	}
	while (str[idx] != '\0' && (str[idx] >= '0' && str[idx] <= '9'))
	{
		res *= 10;
		res += (str[idx] - '0');
		idx++;
	}
	return (res * sign);
}
