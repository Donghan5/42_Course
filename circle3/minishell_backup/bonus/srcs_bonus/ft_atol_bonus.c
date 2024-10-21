/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 21:11:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 22:11:29 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

/*
	Converting string into long int, with error track
*/
long int	ft_atol(char *str, int *err)
{
	int							sign;
	int							i;
	int							j;
	unsigned long long int		res;

	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	j = i;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + (str[i++] - '0');
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] || i - j > 20 || ((sign == -1 && (res - 1) > LONG_MAX) || \
	(sign == 1 && (res > LONG_MAX))))
		*err = 1;
	return ((int)(sign * res) % 256);
}
