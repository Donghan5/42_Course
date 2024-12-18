/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:07:26 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/17 13:48:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	exit_error(int fd, char *line, t_scene *scene, char *message)
{
	if (fd > 2)
		close (fd);
	if (line && message)
	{
		ft_putstr_fd(line, 2);
		ft_putstr_fd("\n", 2);
	}
	smart_free(line);
	if (scene)
		free_scene_safe(scene);
	if (!message)
		exit (1);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	if (!errno)
	{
		ft_putstr_fd("\n", 2);
		exit (1);
	}
	ft_putstr_fd(": ", 2);
	perror(NULL);
	exit (errno);
}

void	smart_free(void *element)
{
	if (element != NULL)
		free(element);
}

void	free_scene_safe(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < scene->cylinder_n || i < scene->sphere_n
		|| i < scene->plane_n || i < scene->light_n
		|| i < scene->cone_n)
	{
		if (i < scene->cylinder_n && scene->cylinder)
			smart_free(scene->cylinder[i]);
		if (i < scene->sphere_n && scene->sphere)
			smart_free(scene->sphere[i]);
		if (i < scene->plane_n && scene->plane)
			smart_free(scene->plane[i]);
		if (i < scene->light_n && scene->light)
			smart_free(scene->light[i]);
		if (i < scene->cone_n && scene->cone)
			smart_free(scene->cone[i]);
		i++;
	}
	smart_free(scene->cylinder);
	smart_free(scene->sphere);
	smart_free(scene->plane);
	smart_free(scene->light);
	smart_free(scene->cone);
}
