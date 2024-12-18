/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:30:21 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	helper function to stock information of coordinate block
	@param
		scene: scene to render
		coord_info: information of coordination(x, y, z)
		pl_idx: index of plane
*/
static void	stock_coordinate(t_scene *scene, char **coord_info, int pl_idx)
{
	scene->plane[pl_idx]->coordinates.x = ft_atod(coord_info[0]);
	scene->plane[pl_idx]->coordinates.y = ft_atod(coord_info[1]);
	scene->plane[pl_idx]->coordinates.z = ft_atod(coord_info[2]);
}

/*
	helper function to stock the normal vector block
	@param
		scene: scene to render
		orient_info: information of the normal vector
		pl_idx: index of plane
*/
static void	stock_normal_vec(t_scene *scene, char **orient_info, int pl_idx)
{
	scene->plane[pl_idx]->normal_vector.x = ft_atod(orient_info[0]);
	scene->plane[pl_idx]->normal_vector.y = ft_atod(orient_info[1]);
	scene->plane[pl_idx]->normal_vector.z = ft_atod(orient_info[2]);
}

/*
	stock element of the plane information blocks
	@param
		scene: scene to render
		line: information of the map which read by gnl
		sep: separated information
		coord_info: coordination information of plane
		orient_info: normal vector information of plane
		rgb_infos: RGB information of plane
*/
void	stock_plane(t_scene *scene, char *line, t_indices *i)
{
	char	**sep;
	char	**coord_info;
	char	**orient_info;
	char	**rgb_infos;

	sep = ft_split(line, ' ');
	if (sep == NULL)
		exit_error(i->fd, line, scene, PARSE_ERR);
	coord_info = ft_split(sep[1], ',');
	if (coord_info == NULL)
		return (free_doub_array(sep), exit_error(i->fd, line, scene, COOR_ERR));
	stock_coordinate(scene, coord_info, i->pl_idx);
	free_doub_array(coord_info);
	orient_info = ft_split(sep[2], ',');
	if (orient_info == NULL)
		return (free_doub_array(sep), exit_error(i->fd, line, scene, ORI_ERR));
	stock_normal_vec(scene, orient_info, i->pl_idx);
	free_doub_array(orient_info);
	rgb_infos = ft_split(sep[3], ',');
	if (rgb_infos == NULL)
		return (free_doub_array(sep),
			exit_error(i->fd, line, scene, RGB));
	if (!stock_rgb(&scene->plane[i->pl_idx]->color, rgb_infos))
		return (free_doub_array(sep), exit_error(i->fd, line, scene, NULL));
	return (free_doub_array(rgb_infos), free_doub_array(sep));
}
