/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:53:25 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/16 15:02:14 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	*init_by_type(int type, t_info *info, int *size,
	double (**intersect_func)(t_ray, void *))
{
	if (type == 0)
	{
		*size = info->scene.sphere_n;
		*intersect_func = (double (*)(t_ray, void *))(&intersect_sphere);
		return (info->scene.sphere);
	}
	else if (type == 1)
	{
		*size = info->scene.plane_n;
		*intersect_func = (double (*)(t_ray, void *))(&intersect_plane);
		return (info->scene.plane);
	}
	else if (type == 2)
	{
		*size = info->scene.cylinder_n;
		*intersect_func = (double (*)(t_ray, void *))(&intersect_cylinder);
		return (info->scene.cylinder);
	}
	else if (type == 3)
	{
		*size = info->scene.cone_n;
		*intersect_func = (double (*)(t_ray, void *))(&intersect_cone);
		return (info->scene.cone);
	}
	return (NULL);
}

void	fill_universal(t_info *info, int type,
	t_ray ray, t_hit_material *closest)
{
	int		i;
	int		size;
	double	t;
	void	**objects;
	double	(*intersect_func)(t_ray, void *);

	i = 0;
	objects = init_by_type(type, info, &size, &intersect_func);
	while (i < size)
	{
		if (objects[i])
		{
			t = (*intersect_func)(ray, objects[i]);
			if (t > EPSILON && t < closest->t)
			{
				closest->t = t;
				closest->type = type;
				closest->index = i;
			}
		}
		i++;
	}
}

/*
	Finds closest object hit
*/
t_hit_material	find_closest(t_ray ray, t_info *info)
{
	t_hit_material	closest;

	closest.t = DBL_MAX;
	closest.type = -1;
	closest.index = -1;
	fill_universal(info, 0, ray, &closest);
	fill_universal(info, 1, ray, &closest);
	fill_universal(info, 2, ray, &closest);
	fill_universal(info, 3, ray, &closest);
	return (closest);
}

/*
	Returns full Phong lights.

	Phong Reflection formula:
	I = Ia + Id + Is
	I = ka×La + kd×Ld×(N·L) + ks×Ls×(R·V)^n

	1. Ambient Component (Ia):
	Ia = ka × La
	where:
	- ka = ambient reflection coefficient (material)
	- La = ambient light intensity

	2. Diffuse Component (Id):
	Id = kd × Ld × (N·L)
	where:
	- kd = diffuse reflection coefficient (material)
	- Ld = diffuse light intensity
	- N = surface normal vector (normalized)
	- L = light direction vector (normalized)
	- (N·L) = dot product, represents cosine of angle between N and L

	3. Specular Component (Is):
	Is = ks × Ls × (R·V)^n
	where:
	- ks = specular reflection coefficient (material)
	- Ls = specular light intensity
	- R = reflection vector = 2(N·L)N - L
	- V = view vector (normalized)
	- n = shininess coefficient (material)
	- (R·V) = dot product, represents cosine of angle between R and V
*/
static t_color	add_lights(t_hit_material closest, t_info *info, t_ray ray)
{
	t_color	result;
	t_color	total_diff_spec[2];
	t_color	ambient_color;

	total_diff_spec[0].r = 0;
	total_diff_spec[0].g = 0;
	total_diff_spec[0].b = 0;
	total_diff_spec[1].r = 0;
	total_diff_spec[1].g = 0;
	total_diff_spec[1].b = 0;
	ambient_color = multiply_colors(closest.base_color,
			info->scene.ambient.color, info->scene.ambient.a_ratio);
	calc_diffuse_spec(closest, info, ray, &total_diff_spec);
	result.r = ambient_color.r + total_diff_spec[0].r + total_diff_spec[1].r;
	result.g = ambient_color.g + total_diff_spec[0].g + total_diff_spec[1].g;
	result.b = ambient_color.b + total_diff_spec[0].b + total_diff_spec[1].b;
	result = normalize_color(result);
	return (result);
}

/*
	Raytracing formula

	P(t) = O + tD
	Where:
	- P is point along ray
	- O is ray origin
	- D is ray direction
	- t is distance along ray
*/
t_color	trace_ray(t_ray ray, t_info *info, int depth)
{
	t_color			result;
	t_hit_material	closest;
	t_color			refl;

	if (depth >= MAX_DEPTH)
		return ((t_color){0, 0, 0});
	closest = find_closest(ray, info);
	if (closest.t >= DBL_MAX)
		return ((t_color){0, 0, 0});
	closest.hit_point = vec3_add(ray.origin,
			vec3_mul(ray.direction, closest.t));
	fill_material(&closest, info);
	result = add_lights(closest, info, ray);
	if (!info->is_mirror || closest.reflectivity <= 0.0 || depth >= MAX_DEPTH)
		return (result);
	ray.origin = vec3_add(closest.hit_point, vec3_mul(closest.normal, EPSILON));
	ray.direction = vec3_reflect(ray.direction, closest.normal);
	refl = trace_ray(ray, info, depth + 1);
	result.r = result.r * (1 - closest.reflectivity)
		+ refl.r * closest.reflectivity;
	result.g = result.g * (1 - closest.reflectivity)
		+ refl.g * closest.reflectivity;
	result.b = result.b * (1 - closest.reflectivity)
		+ refl.b * closest.reflectivity;
	return (result);
}
