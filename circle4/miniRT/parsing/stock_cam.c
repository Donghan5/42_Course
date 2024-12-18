/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_cam.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:06:53 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 13:53:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	stock coordinate information
	@param
		scene: to render
		coord_info: coordinate information
*/
static void	stock_coord(t_scene *scene, char **coord_info)
{
	scene->camera.coordinates.x = ft_atod(coord_info[0]);
	scene->camera.coordinates.y = ft_atod(coord_info[1]);
	scene->camera.coordinates.z = ft_atod(coord_info[2]);
}

/*
	stock orientation information
	@param
		scene: to render
		orient_info: orientation information
*/
static void	stock_orient(t_scene *scene, char **orient_info)
{
	scene->camera.orientation.x = ft_atod(orient_info[0]);
	scene->camera.orientation.y = ft_atod(orient_info[1]);
	scene->camera.orientation.z = ft_atod(orient_info[2]);
}

/*
	stock the informations of camera to structure
	@param
		scene: to render
		line: information of the map which read by gnl
		sep_info: separated information (meaningful value)
		coord_info: coordination information of camera
		orient_info: orientation vector information of camera
*/
void	stock_cam(int fd, t_scene *scene, char *line)
{
	char	**sep_info;
	char	**coord_info;
	char	**orient_info;

	sep_info = ft_split(line, ' ');
	if (sep_info == NULL)
		exit_error(fd, line, scene, PARSE_ERR);
	coord_info = ft_split((char *)sep_info[1], ',');
	if (coord_info == NULL)
		return (free_doub_array(sep_info),
			exit_error(fd, line, scene, COOR_ERR));
	stock_coord(scene, coord_info);
	free_doub_array(coord_info);
	orient_info = ft_split((char *)sep_info[2], ',');
	if (orient_info == NULL)
		return (free_doub_array(sep_info),
			exit_error(fd, line, scene, ORI_ERR));
	stock_orient(scene, orient_info);
	scene->camera.c_view = ft_atod(sep_info[3]);
	free_doub_array(orient_info);
	free_doub_array(sep_info);
}
