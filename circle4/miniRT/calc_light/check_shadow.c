/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_shadow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:52:20 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/11 15:41:01 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_t_in_shadow(double t, double dist_to_light)
{
	if (t > EPSILON && t < (dist_to_light) && !is_equal(t, dist_to_light))
		return (1);
	return (0);
}

int	shadow_found(t_info *info, int i, t_ray shadow_ray, double dist_to_light)
{
	double	t;

	t = 0;
	if (i < info->scene.sphere_n && info->scene.sphere[i])
		t = intersect_sphere(shadow_ray, info->scene.sphere[i]);
	if (!is_t_in_shadow(t, dist_to_light)
		&& i < info->scene.plane_n && info->scene.plane[i])
		t = intersect_plane(shadow_ray, info->scene.plane[i]);
	if (!is_t_in_shadow(t, dist_to_light)
		&& i < info->scene.cylinder_n && info->scene.cylinder[i])
		t = intersect_cylinder(shadow_ray, info->scene.cylinder[i]);
	if (!is_t_in_shadow(t, dist_to_light)
		&& i < info->scene.cone_n && info->scene.cone[i])
		t = intersect_cone(shadow_ray, info->scene.cone[i]);
	return (is_t_in_shadow(t, dist_to_light));
}

double	check_shadow(t_info *info, t_vec3 hit_point, t_vec3 light_pos)
{
	t_ray	shadow_ray;
	t_vec3	to_light;
	double	dist_to_light;
	int		i;

	i = 0;
	shadow_ray.origin = hit_point;
	shadow_ray.direction = vec3_normalize(vec3_sub(light_pos, hit_point));
	to_light = vec3_sub(light_pos, hit_point);
	dist_to_light = sqrt(vec3_dot(to_light, to_light));
	while (i < info->scene.sphere_n || i < info->scene.plane_n
		|| i < info->scene.cylinder_n || i < info->scene.cone_n)
	{
		if (shadow_found(info, i, shadow_ray, dist_to_light))
			return (0.0);
		i++;
	}
	return (1.0);
}
