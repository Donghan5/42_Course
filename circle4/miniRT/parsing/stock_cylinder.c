/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:39:04 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	helper function of stock coordinate and axis vector
	@param
		scene: scene to render
		coord_info: information of the coordinate (x, y, z)
		orient_info: information of axis vector
		cy_idx: index of cylinder
*/
static void	stock_co(t_scene *scene, char **coord_info, \
					char **orient_info, int cy_idx)
{
	scene->cylinder[cy_idx]->coordinates.x = ft_atod(coord_info[0]);
	scene->cylinder[cy_idx]->coordinates.y = ft_atod(coord_info[1]);
	scene->cylinder[cy_idx]->coordinates.z = ft_atod(coord_info[2]);
	scene->cylinder[cy_idx]->axis_vector.x = ft_atod(orient_info[0]);
	scene->cylinder[cy_idx]->axis_vector.y = ft_atod(orient_info[1]);
	scene->cylinder[cy_idx]->axis_vector.z = ft_atod(orient_info[2]);
}

/*
	helper function of diameter and height
	@param
		scene: to render
		sep_info: separated information
		cy_idx: index of the cylinder
*/
static void	stock_dia_hei(t_scene *scene, char **sep_info, int cy_idx)
{
	scene->cylinder[cy_idx]->cy_diameter = ft_atod(sep_info[3]);
	scene->cylinder[cy_idx]->cy_height = ft_atod(sep_info[4]);
}

/*
	stock information of cylinder to structure
	@param
		scene: scene to render
		line: information of the map which read by gnl
		sep: separated information
		coord_info: coordinate information of cylinder
		orient_info: normal vector information of cylinder
		rgb_infos: RGB information of cylinder
*/
void	stock_cylinder(t_scene *scene, char *line, t_indices *i)
{
	char	**sep;
	char	**coord;
	char	**orient_info;
	char	**rgb_infos;

	sep = ft_split(line, ' ');
	if (sep == NULL)
		exit_error(i->fd, line, scene, PARSE_ERR);
	coord = ft_split(sep[1], ',');
	if (coord == NULL)
		return (free_doub_array(sep), exit_error(i->fd, line, scene, COOR_ERR));
	orient_info = ft_split((char *)sep[2], ',');
	if (orient_info == NULL)
		return (free_doub_array(coord),
			free_doub_array(sep), exit_error(i->fd, line, scene, ORI_ERR));
	stock_co(scene, coord, orient_info, i->cy_idx);
	free_doub_array(coord);
	free_doub_array(orient_info);
	stock_dia_hei(scene, sep, i->cy_idx);
	rgb_infos = ft_split(sep[5], ',');
	if (rgb_infos == NULL)
		return (free_doub_array(sep), exit_error(i->fd, line, scene, RGB));
	if (!stock_rgb(&scene->cylinder[i->cy_idx]->color, rgb_infos))
		return (free_doub_array(sep), exit_error(i->fd, line, scene, NULL));
	return (free_doub_array(rgb_infos), free_doub_array(sep));
}
