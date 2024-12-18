/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shape.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:20:51 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 13:49:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	initailze light
	@param
		scene: the structure which want to init
*/
void	init_light(t_scene *scene)
{
	int	l_idx;

	l_idx = 0;
	scene->light = malloc(scene->light_n * sizeof(t_light *));
	if (!scene->light)
		exit_error(0, NULL, scene, "Fail to dynamic allocatie t_light");
	while (l_idx < scene->light_n)
	{
		scene->light[l_idx] = ft_calloc(1, sizeof(t_light));
		if (!scene->light[l_idx])
			exit_error(0, NULL, scene, "Fail to dynamic allocat t_light");
		scene->light[l_idx]->color.r = 0;
		scene->light[l_idx]->color.g = 0;
		scene->light[l_idx]->color.b = 0;
		scene->light[l_idx]->coordinates.x = 0.0;
		scene->light[l_idx]->coordinates.y = 0.0;
		scene->light[l_idx]->coordinates.z = 0.0;
		scene->light[l_idx]->l_brightness = 0.0;
		l_idx++;
	}
}

/*
	init sphere part
	@param
		scene: the structure which want to init
*/
void	init_sphere(t_scene *scene)
{
	int	sp_idx;

	sp_idx = 0;
	scene->sphere = malloc(scene->sphere_n * sizeof(t_sphere *));
	if (!scene->sphere)
		exit_error(0, NULL, scene, "Fail allocate t_sphere scope all");
	while (sp_idx < scene->sphere_n)
	{
		scene->sphere[sp_idx] = ft_calloc(1, sizeof(t_sphere));
		if (!scene->sphere[sp_idx])
			exit_error(0, NULL, scene, "Fail allocate t_sphere scope index");
		scene->sphere[sp_idx]->color.r = 0;
		scene->sphere[sp_idx]->color.g = 0;
		scene->sphere[sp_idx]->color.b = 0;
		scene->sphere[sp_idx]->coordinates.x = 0.0;
		scene->sphere[sp_idx]->coordinates.y = 0.0;
		scene->sphere[sp_idx]->coordinates.z = 0.0;
		scene->sphere[sp_idx]->sp_diameter = 0;
		sp_idx++;
	}
}

/*
	init plane part
	@param
		scene: the structure which want to init
*/
void	init_plane(t_scene *scene)
{
	int	pl_idx;

	pl_idx = 0;
	scene->plane = malloc(scene->plane_n * sizeof(t_plane *));
	if (!scene->plane)
		exit_error(0, NULL, scene, "Fail dynamic allocate t_plane");
	while (pl_idx < scene->plane_n)
	{
		scene->plane[pl_idx] = ft_calloc(1, sizeof(t_plane));
		if (!scene->plane[pl_idx])
			exit_error(0, NULL, scene, "Fail dynamic allocate t_plane");
		scene->plane[pl_idx]->color.r = 0;
		scene->plane[pl_idx]->color.g = 0;
		scene->plane[pl_idx]->color.b = 0;
		scene->plane[pl_idx]->coordinates.x = 0.0;
		scene->plane[pl_idx]->coordinates.y = 0.0;
		scene->plane[pl_idx]->coordinates.z = 0.0;
		scene->plane[pl_idx]->normal_vector.x = 0.0;
		scene->plane[pl_idx]->normal_vector.y = 0.0;
		scene->plane[pl_idx]->normal_vector.z = 0.0;
		pl_idx++;
	}
}

/*
	init cylinder part
	@param
		scene: the structure which want to init
*/
void	init_cylinder(t_scene *scene)
{
	int	cy_idx;

	cy_idx = 0;
	scene->cylinder = malloc(scene->cylinder_n * sizeof(t_cylinder *));
	if (!scene->cylinder)
		exit_error(0, NULL, scene, "Fail to dynamic allocate t_cylinder");
	while (cy_idx < scene->cylinder_n)
	{
		scene->cylinder[cy_idx] = ft_calloc(1, sizeof(t_cylinder));
		if (!scene->cylinder[cy_idx])
			exit_error(0, NULL, scene, "Fail dynamic allocate t_cylinder");
		scene->cylinder[cy_idx]->axis_vector.x = 0.0;
		scene->cylinder[cy_idx]->axis_vector.y = 0.0;
		scene->cylinder[cy_idx]->axis_vector.z = 0.0;
		scene->cylinder[cy_idx]->color.r = 0;
		scene->cylinder[cy_idx]->color.g = 0;
		scene->cylinder[cy_idx]->color.b = 0;
		scene->cylinder[cy_idx]->coordinates.x = 0.0;
		scene->cylinder[cy_idx]->coordinates.y = 0.0;
		scene->cylinder[cy_idx]->coordinates.z = 0.0;
		scene->cylinder[cy_idx]->cy_diameter = 0.0;
		scene->cylinder[cy_idx]->cy_height = 0.0;
		cy_idx++;
	}
}

/*
	init cone part
	@param
		scene: the structure which want to init
*/
void	init_cone(t_scene *scene)
{
	int	co_idx;

	co_idx = 0;
	scene->cone = malloc(scene->cone_n * sizeof(t_cone *));
	if (!scene->cone)
		exit_error(0, NULL, scene, "Fail to dynamic allocate t_cone");
	while (co_idx < scene->cone_n)
	{
		scene->cone[co_idx] = ft_calloc(1, sizeof(t_cone));
		if (!scene->cone[co_idx])
			exit_error(0, NULL, scene, "Fail dynamic allocate t_cone");
		scene->cone[co_idx]->axis_vector.x = 0.0;
		scene->cone[co_idx]->axis_vector.y = 0.0;
		scene->cone[co_idx]->axis_vector.z = 0.0;
		scene->cone[co_idx]->color.r = 0;
		scene->cone[co_idx]->color.g = 0;
		scene->cone[co_idx]->color.b = 0;
		scene->cone[co_idx]->coordinates.x = 0.0;
		scene->cone[co_idx]->coordinates.y = 0.0;
		scene->cone[co_idx]->coordinates.z = 0.0;
		scene->cone[co_idx]->co_diameter = 0.0;
		scene->cone[co_idx]->co_height = 0.0;
		co_idx++;
	}
}
