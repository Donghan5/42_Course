/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:06:48 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	stock ambient information block
	@param
		scene: to render
		info_map: information of the map which read by gnl
		sep_info: separate information (meaningful value)
		rgb_infos: RGB information
*/
void	stock_ambient(int fd, t_scene *scene, char *info_map)
{
	char	**sep_info;
	char	**rgb_infos;

	sep_info = ft_split(info_map, ' ');
	if (sep_info == NULL)
		exit_error(fd, info_map, scene, PARSE_ERR);
	scene->ambient.a_ratio = ft_atod(sep_info[1]);
	rgb_infos = ft_split((char *)sep_info[2], ',');
	if (rgb_infos == NULL)
		return (free_doub_array(sep_info), \
			exit_error(fd, info_map, scene, RGB));
	if (!stock_rgb(&scene->ambient.color, rgb_infos))
		return (free_doub_array(sep_info),
			exit_error(fd, info_map, scene, NULL));
	free_doub_array(rgb_infos);
	free_doub_array(sep_info);
}
