/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:07:02 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	stock coordinate informations
	@param
		scene: to render
		coord_info: information of the coordinate
		sp_idx: index of the sphere
*/
static void	stock_coord(t_scene *scene, char **coord_info, int sp_idx)
{
	scene->sphere[sp_idx]->coordinates.x = ft_atod(coord_info[0]);
	scene->sphere[sp_idx]->coordinates.y = ft_atod(coord_info[1]);
	scene->sphere[sp_idx]->coordinates.z = ft_atod(coord_info[2]);
}

/*
	stock element of sphere
	@param
		scene: to render
		line: information of the map read by gnl
		sep: separated information by meaningful value
		coord_info: coordination information of sphere
		rgb_infos: RGB information of sphere
*/
void	stock_sphere(t_scene *scene, char *line, t_indices *indices)
{
	char	**sep;
	char	**coord_info;
	char	**rgb_infos;

	sep = ft_split(line, ' ');
	if (sep == NULL)
		exit_error(indices->fd, line, scene, PARSE_ERR);
	coord_info = ft_split(sep[1], ',');
	if (coord_info == NULL)
		return (free_doub_array(sep),
			exit_error(indices->fd, line, scene, COOR_ERR));
	stock_coord(scene, coord_info, indices->sp_idx);
	free_doub_array(coord_info);
	scene->sphere[indices->sp_idx]->sp_diameter = ft_atod(sep[2]);
	rgb_infos = ft_split((char *)sep[3], ',');
	if (rgb_infos == NULL)
		return (free_doub_array(sep),
			exit_error(indices->fd, line, scene, RGB));
	if (!stock_rgb(&scene->sphere[indices->sp_idx]->color, rgb_infos))
		return (free_doub_array(sep),
			exit_error(indices->fd, line, scene, NULL));
	free_doub_array(rgb_infos);
	free_doub_array(sep);
}
