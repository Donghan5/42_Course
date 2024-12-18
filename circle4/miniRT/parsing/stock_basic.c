/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_basic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:16:25 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 13:52:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	checking the range of the RGB value
	@param
		color: t_color to check r,g,b values ranges
	@return
		1: success
		0: fail
*/
static t_bool	check_range(t_color *color)
{
	if (color->r > 255 || color->r < 0
		|| color->g > 255 || color->g < 0
		|| color->b > 255 || color->b < 0)
		return (0);
	return (1);
}

/*
	store all rgb information
	check double or out of range cases
	@param
		color: color structure
		rgb_infos: information of RGB
	@return
		1: success
		0: fail
*/
t_bool	stock_rgb(t_color *color, char **rgb_infos)
{
	if (ft_strchr(rgb_infos[0], '.')
		|| ft_strchr(rgb_infos[1], '.')
		|| ft_strchr(rgb_infos[2], '.'))
	{
		free_doub_array(rgb_infos);
		ft_putstr_fd("Error\nInvalid rgb value type\n", 2);
		return (0);
	}
	(*color).r = ft_atoi(rgb_infos[0]);
	(*color).g = ft_atoi(rgb_infos[1]);
	(*color).b = ft_atoi(rgb_infos[2]);
	if (!check_range(color))
	{
		free_doub_array(rgb_infos);
		ft_putstr_fd("Error\nRGB value is out of range\n", 2);
		return (0);
	}
	return (1);
}

/*
	part of ambient, camera
	@param
		type: type of the map_infos[1]
		scene: structure to stock the infos
		info_map: numeric information of the map
*/
void	stock_infos(int fd, int type, t_scene *scene, char *info_map)
{
	if (type == 1)
		stock_ambient(fd, scene, info_map);
	else if (type == 2)
		stock_cam(fd, scene, info_map);
}
