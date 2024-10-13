/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 07:18:46 by donghank          #+#    #+#             */
/*   Updated: 2024/04/01 10:00:09 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int	ft_strlen(char *str);
int	ft_atoi_base(char *str, char *base);

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	is_error(char *str, int len)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	if (len <= 1)
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] == '+' || str[i] == '-')
			return (1);
		if (str[i] < 32 || str[i] > 126)
			return (1);
		j = i + 1;
		while (str[j] != '\0')
		{
			if (str[i] == str[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	len_nbr(long long nb, int base_len)
{
	int	size;

	if (!nb)
		return (1);
	size = 0;
	if (nb < 0)
		++size;
	while (nb)
	{
		nb /= base_len;
		++size;
	}
	return (size);
}

char	*ft_putnbr_base(int nbr, char *base)
{
	char		*str;
	long long	nb;
	int			i;

	nb = nbr;
	i = len_nbr(nb, base_len);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str || is_error(base, base_len))
		return (NULL);
	str[i] = '\0';
	if (nb == 0)
		str[0] = base[0];
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		str[--i] = base[nb % ft_strlen(base)];
		nb /= ft_strlen(base);
	}
	return (str);
}

char	*ft_convert_base(char *nbr, char *base_from, char *base_to)
{
	int		number;
	char	*str;
	int		from_len;
	int		to_len;

	from_len = ft_strlen(base_from);
	to_len = ft_strlen(base_to);
	if (is_error(base_from, from_len) || is_error(base_to, to_len))
		return (NULL);
	number = ft_atoi_base(nbr, base_from);
	str = ft_putnbr_base(number, base_to);
	return (str);
}
