/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:12:42 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 13:56:44 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	initailze all element of scene
	@param
		scene: want to init
*/
void	init_scene(char *path, t_scene *scene)
{
	scene->plane_n = 0;
	scene->sphere_n = 0;
	scene->cylinder_n = 0;
	scene->light_n = 0;
	scene->cone_n = 0;
	count_objs(path, scene);
	init_ambient(scene);
	init_camera(scene);
	init_light(scene);
	init_sphere(scene);
	init_plane(scene);
	init_cylinder(scene);
	init_cone(scene);
}

/*
	initailze ambient
	@param
		scene: want to init
*/
void	init_ambient(t_scene *scene)
{
	scene->ambient.a_ratio = 0.0;
	scene->ambient.color.r = 0;
	scene->ambient.color.g = 0;
	scene->ambient.color.b = 0;
}

/*
	initailze camera
	@param
		scene: want to init
*/
void	init_camera(t_scene *scene)
{
	scene->camera.c_view = 0.0;
	scene->camera.coordinates.x = 0.0;
	scene->camera.coordinates.y = 0.0;
	scene->camera.coordinates.z = 0.0;
	scene->camera.orientation.x = 0.0;
	scene->camera.orientation.y = 0.0;
	scene->camera.orientation.z = 0.0;
}

/*
	init all of the count
	@param
		count: structure of count which want to init
*/
void	init_count(t_count *count)
{
	count->a_count = 0;
	count->c_count = 0;
	count->l_count = 0;
	count->sp_count = 0;
	count->pl_count = 0;
	count->cy_count = 0;
	count->co_count = 0;
}

/*
	init all the indexs
	@param
		indices: sturucture of the index
*/
void	init_indices(t_indices *indices)
{
	indices->cy_idx = 0;
	indices->pl_idx = 0;
	indices->sp_idx = 0;
	indices->l_idx = 0;
	indices->co_idx = 0;
	indices->fd = 0;
}
