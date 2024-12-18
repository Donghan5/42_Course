/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:49:17 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 17:05:38 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_equal(double a, double b)
{
	if (fabs(a - b) <= EPSILON * fabs(a))
		return (1);
	return (0);
}

t_color	normalize_color(t_color color)
{
	color.r = (int)fmin(255, fmax(0, color.r));
	color.g = (int)fmin(255, fmax(0, color.g));
	color.b = (int)fmin(255, fmax(0, color.b));
	return (color);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	len;

	len = ft_strlen(s);
	write(fd, s, len);
}
