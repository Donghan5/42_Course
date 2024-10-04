/* During the exam session there is not norm rules */

int	ft_atoi(char *str)
{
	int idx = 0;
	int	sign = 1;
	int	res = 0;

	while (str[idx] == ' ' || (str[idx] >= 9 && str[idx] <= 13))
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

/*
another sol
int ft_atoi(const char *str)
{
	int sign = 1;
	int res = 0;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (sign * res);
}
*/
