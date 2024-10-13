/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:48:06 by donghank          #+#    #+#             */
/*   Updated: 2024/04/01 10:00:19 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	is_error(char *str, int len);
int	ft_putnbr_base(int nbr, char *base);

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

int	negative_whitespace(char *str, int *ptr)
{
	int	pos;
	int	i;

	pos = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			pos *= -1;
		i++;
	}
	*ptr = i;
	return (pos);
}

int	base_index(char c, char *base)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (c == base[i])
			return (i);
		i++;
	}
	return (-1);
}

int	ft_atoi_base(char *str, char *base)
{
	int	i;
	int	res;
	int	len;
	int	sign;

	res = 0;
	i = 0;
	len = ft_strlen(base);
	sign = negative_whitespace(str, &i);
	if (is_error(base, len) == 1)
		return (0);
	while (str[i] && base_index(str[i], base) != -1)
	{
		res = res * len + base_index(str[i], base);
		i++;
	}
	return (sign * res);
}
