/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 00:48:43 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	stock coordinate information block
	@param
		scene: to render
		coord_info: information of coordinate
		l_idx: index of light_n
*/
static void	stock_coord(t_scene *scene, char **coord_info, int l_idx)
{
	scene->light[l_idx]->coordinates.x = ft_atod(coord_info[0]);
	scene->light[l_idx]->coordinates.y = ft_atod(coord_info[1]);
	scene->light[l_idx]->coordinates.z = ft_atod(coord_info[2]);
}

/*
	stock light information block
	@param
		scene: to render
		info_map: information map read by gnl
		sep_info: separated information (meaningful value)
		coord_info: coordinate information of light
		rgb_infos: RGB information of light
*/
void	stock_light(t_scene *scene, char *info_map, t_indices *indices)
{
	char	**sep_info;
	char	**coord_info;
	char	**rgb_infos;

	sep_info = ft_split(info_map, ' ');
	if (sep_info == NULL)
		exit_error(indices->fd, info_map, scene, PARSE_ERR);
	coord_info = ft_split((char *)sep_info[1], ',');
	if (coord_info == NULL)
		return (free_doub_array(sep_info),
			exit_error(indices->fd, info_map, scene, COOR_ERR));
	stock_coord(scene, coord_info, indices->l_idx);
	free_doub_array(coord_info);
	scene->light[indices->l_idx]->l_brightness = ft_atod(sep_info[2]);
	rgb_infos = ft_split((char *)sep_info[3], ',');
	if (rgb_infos == NULL)
		return (free_doub_array(sep_info),
			exit_error(indices->fd, info_map, scene, RGB));
	if (!stock_rgb(&scene->light[indices->l_idx]->color, rgb_infos))
		return (free_doub_array(sep_info),
			exit_error(indices->fd, info_map, scene, NULL));
	free_doub_array(rgb_infos);
	free_doub_array(sep_info);
}
