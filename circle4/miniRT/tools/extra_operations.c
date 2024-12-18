/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:50:08 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/10 13:41:47 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	t_color_add(t_color a, t_color b)
{
	return ((t_color){a.r + b.r, a.g + b.g, a.b + b.b});
}

double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = sqrt(vec3_dot(v, v));
	if (len < 1e-6)
		return (vec3(0, 0, 0));
	return (vec3_mul(v, 1.0 / len));
}

t_vec3	vec3_reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mul(n, 2 * vec3_dot(v, n))));
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return (vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		));
}
