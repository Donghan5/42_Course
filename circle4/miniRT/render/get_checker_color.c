/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_checker_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:19:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 18:54:28 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_color	get_checker_color(t_vec3 hit_point,
	t_vec3 normal, t_color base_color)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	double	u;
	double	v;
	int		pattern;

	if (fabs(normal.y) > 0.9)
		u_axis = vec3_normalize(vec3_cross(normal, vec3(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(normal, vec3(0, 1, 0)));
	v_axis = vec3_normalize(vec3_cross(normal, u_axis));
	u = vec3_dot(hit_point, u_axis);
	v = vec3_dot(hit_point, v_axis);
	pattern = (int)(floor(u / 15.0) + floor(v / 15.0));
	if (pattern % 2 != 0)
	{
		base_color.r = base_color.r / 2;
		base_color.g = base_color.g / 2;
		base_color.b = base_color.b / 2;
	}
	return (base_color);
}
