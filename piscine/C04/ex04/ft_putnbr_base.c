/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 12:58:13 by donghank          #+#    #+#             */
/*   Updated: 2024/03/25 07:08:12 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

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

int	is_error(char *str, int len)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	if (len <= 1)
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] == '+' || str[i] == '-')
			return (0);
		if (str[i] < 32 || str[i] > 126)
			return (0);
		j = i + 1;
		while (str[j] != '\0')
		{
			if (str[i] == str[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	ft_putnbr_base_print(int nbr, char *base)
{
	int	base_len;

	base_len = ft_strlen(base);
	if (nbr >= base_len)
	{
		ft_putnbr_base_print((nbr / base_len), base);
		ft_putnbr_base_print((nbr % base_len), base);
	}
	else if (nbr < 0)
	{
		if (nbr == -2147483648)
		{
			ft_putchar('-');
			ft_putchar('2');
			ft_putnbr_base_print(147483648, base);
		}
		else
		{
			ft_putchar('-');
			nbr *= -1;
			ft_putnbr_base_print(nbr, base);
		}
	}
	else if (nbr < base_len)
		ft_putchar(base[nbr]);
}

void	ft_putnbr_base(int nbr, char *base)
{
	int		base_len;
	int		error;

	base_len = ft_strlen(base);
	error = is_error(base, base_len);
	if (error == 1)
	{
		ft_putnbr_base_print(nbr, base);
	}
}
