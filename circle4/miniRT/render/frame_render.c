/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:48:45 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 18:29:50 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_render(t_info *info)
{
	if (info->moving_text || info->rotate_camera || info->is_input)
	{
		info->render_type = LOW_RENDER;
		return (1);
	}
	if (info->render_type != NO_RENDER)
		return (1);
	return (0);
}

int	render_next_frame(t_info *info)
{
	if (!is_render(info))
		return (0);
	render_scene(info);
	mlx_do_sync(info->mlx);
	mlx_put_image_to_window(info->mlx, info->win, info->img->img, 0, 0);
	print_info_block(info);
	mlx_destroy_image(info->mlx, info->img->img);
	info->img->img = mlx_new_image(info->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	info->img->addr = mlx_get_data_addr(info->img->img,
			&info->img->bits_per_pixel, &info->img->line_length,
			&info->img->endian);
	return (0);
}
