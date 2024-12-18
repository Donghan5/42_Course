/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:01:06 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:10:00 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	helper function to save the line
	basically, free the map_line and exit the loop
	@param
		map_line: to free
		msg: error message (user input)
*/
void	handle_error_free(char *map_line, char *msg)
{
	ft_putstr_fd(map_line, 2);
	free(map_line);
	ft_putstr_fd("\n", 2);
	handle_error(msg);
}

/*
	helper function of process_parse()
	@param
		type: the type of information
		map_line: information of the map which read by gnl
		scene: information to render
		indices: the index structure
*/
static void	parse_obj(int type, char *map_line, \
				t_scene *scene, t_indices *indices)
{
	if (type == 3)
	{
		if (indices->l_idx >= scene->light_n)
			exit_error(indices->fd, map_line, scene,
				"Index is out of range in light");
		stock_light(scene, map_line, indices);
		indices->l_idx++;
	}
	else if (type == 4)
	{
		if (indices->pl_idx >= scene->plane_n)
			exit_error(indices->fd, map_line, scene,
				"Index is out of range in plane");
		stock_plane(scene, map_line, indices);
		indices->pl_idx++;
	}
}

/*
	helper function of process_parse()
	@param
		type: the type of information
		map_line: information of the map which read by gnl
		scene: information to render
		indices: the index structure
*/
static void	parse_obj_advance(int type, char *map_line, \
				t_scene *scene, t_indices *indices)
{
	if (type == 5)
	{
		if (indices->sp_idx >= scene->sphere_n)
			exit_error(indices->fd, map_line, scene,
				"Index is out of range in sphere");
		stock_sphere(scene, map_line, indices);
		indices->sp_idx++;
	}
	else if (type == 6)
	{
		if (indices->cy_idx >= scene->cylinder_n)
			exit_error(indices->fd, map_line, scene,
				"Index is out of range in cylinder");
		stock_cylinder(scene, map_line, indices);
		indices->cy_idx++;
	}
	else if (type == 7)
	{
		if (indices->co_idx >= scene->cone_n)
			exit_error(indices->fd, map_line, scene,
				"Index is out of range in cone");
		stock_cone(scene, map_line, indices);
		indices->co_idx++;
	}
}

/*
	process of the stock all element in structure (helper function)
	@param
		map_line: information of the map which read by gnl
		scene: information to render
		indices: structure which contain all index of sp, pl and cy
		type: the type of the information (ambient, cam, light etc)
*/
void	process_parse(char *map_line, t_scene *scene,
	t_indices *indices)
{
	int	type;

	type = get_type(map_line);
	if (type == 1 || type == 2)
		stock_infos(indices->fd, type, scene, map_line);
	else if (type >= 3 && type <= 7)
	{
		parse_obj(type, map_line, scene, indices);
		parse_obj_advance(type, map_line, scene, indices);
	}
	else if (type == 0)
		exit_error(indices->fd, map_line, scene, "Invalid type!");
}

/*
	checking validity of the map and contain the process of stock
	@param
		map_line: information of the map
		scene: to render
		indices: the structure which contain all index of multiple element
*/
void	handle_map_check(char *map_line, t_scene *scene, t_indices *indices)
{
	if (is_empty_or_comment(map_line))
		return ;
	process_parse(map_line, scene, indices);
}
