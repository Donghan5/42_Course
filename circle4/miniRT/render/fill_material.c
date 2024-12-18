/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_material.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:32:31 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/16 15:02:12 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	fill_sphere(t_hit_material *closest, t_info *info)
{
	t_sphere	*sphere;

	sphere = info->scene.sphere[closest->index];
	closest->normal = vec3_normalize(vec3_sub(closest->hit_point,
				sphere->coordinates));
	closest->base_color.r = sphere->color.r;
	closest->base_color.g = sphere->color.g;
	closest->base_color.b = sphere->color.b;
	closest->shininess = 50.0;
	closest->spec_intensity = 0.4;
	closest->diffuse_strength = 0.6;
	closest->reflectivity = 0.5;
}

static void	fill_plane(t_hit_material *closest, t_info *info)
{
	t_plane	*plane;

	plane = info->scene.plane[closest->index];
	closest->normal = vec3_normalize(plane->normal_vector);
	if (info->is_checker)
		closest->base_color = get_checker_color(closest->hit_point,
				closest->normal, plane->color);
	else
		closest->base_color = plane->color;
	closest->shininess = 1.0;
	closest->spec_intensity = 0.1;
	closest->diffuse_strength = 0.8;
	closest->reflectivity = 0.0;
}

static void	fill_cylinder(t_hit_material *closest, t_info *info)
{
	t_cylinder	*cyl;
	t_vec3		axis;
	double		height;

	cyl = info->scene.cylinder[closest->index];
	axis = vec3_normalize(cyl->axis_vector);
	height = vec3_dot(vec3_sub(closest->hit_point, cyl->coordinates), axis);
	if ((fabs(height) < EPSILON || fabs(height - cyl->cy_height) < EPSILON)
		&& height < EPSILON)
			closest->normal = vec3_mul(axis, -1.0);
	else if ((fabs(height) < EPSILON
			|| fabs(height - cyl->cy_height) < EPSILON))
			closest->normal = axis;
	else
		closest->normal = vec3_normalize(vec3_sub(closest->hit_point,
					vec3_add(cyl->coordinates, vec3_mul(axis,
							vec3_dot(vec3_sub(closest->hit_point,
									cyl->coordinates), axis)))));
	closest->base_color.r = cyl->color.r;
	closest->base_color.g = cyl->color.g;
	closest->base_color.b = cyl->color.b;
	closest->shininess = 30.0;
	closest->spec_intensity = 0.3;
	closest->diffuse_strength = 0.7;
	closest->reflectivity = 0.5;
}

static void	fill_cone(t_hit_material *closest, t_info *info)
{
	t_cone	*co;
	t_vec3	axis;
	t_vec3	to_hit;
	double	height;

	co = info->scene.cone[closest->index];
	axis = vec3_normalize(co->axis_vector);
	to_hit = vec3_sub(closest->hit_point, co->coordinates);
	height = vec3_dot(to_hit, axis);
	if (fabs(height - co->co_height / 2.0) < EPSILON)
		closest->normal = axis;
	else if (fabs(height + co->co_height / 2.0) < EPSILON)
		closest->normal = vec3_mul(axis, -1.0);
	else
		closest->normal = vec3_normalize(vec3_sub(closest->hit_point,
					vec3_add(co->coordinates,
						vec3_mul(axis, vec3_dot(vec3_sub(closest->hit_point,
									co->coordinates), axis)))));
	closest->base_color.r = co->color.r;
	closest->base_color.g = co->color.g;
	closest->base_color.b = co->color.b;
	closest->shininess = 10.0;
	closest->spec_intensity = 0.4;
	closest->diffuse_strength = 0.4;
	closest->reflectivity = 0.0;
}

/*
	Fills material properties of object hit
*/
void	fill_material(t_hit_material *closest, t_info *info)
{
	if (closest->type == 0)
		fill_sphere(closest, info);
	else if (closest->type == 1)
		fill_plane(closest, info);
	else if (closest->type == 2)
		fill_cylinder(closest, info);
	else if (closest->type == 3)
		fill_cone(closest, info);
}
