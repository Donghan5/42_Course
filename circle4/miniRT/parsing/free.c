/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:37:09 by donghank          #+#    #+#             */
/*   Updated: 2024/12/12 12:38:09 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	free double array
	@param
		strs: double array which want to free
*/
void	free_doub_array(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

/*
	helper function to free specific index
	@param
		strs: to free
		count: index to free
*/
static void	free_doub(void **strs, int count)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (i < count)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_just_scene(t_scene *scene)
{
	if (!scene)
		return ;
	free_doub((void **)scene->light, scene->light_n);
	free_doub((void **)scene->plane, scene->plane_n);
	free_doub((void **)scene->sphere, scene->sphere_n);
	free_doub((void **)scene->cylinder, scene->cylinder_n);
	free_doub((void **)scene->cone, scene->cone_n);
}
