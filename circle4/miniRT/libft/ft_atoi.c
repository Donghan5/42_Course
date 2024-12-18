/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:21:12 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/16 14:03:15 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	calc_tens(int n)
{
	if (n == 1)
		return (1);
	return (calc_tens(n - 1) * 10);
}

static int	ft_numlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] >= '0' && s[i] <= '9')
		i++;
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	sign;
	int	numlen;

	res = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	numlen = ft_numlen(nptr);
	while (numlen)
	{
		res += (*nptr - '0') * calc_tens(numlen);
		numlen--;
		nptr++;
	}
	return (res * sign);
}
