/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_info_block.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:07:06 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 18:31:36 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	print_checker(t_info *info, unsigned int gray, unsigned int green)
{
	unsigned int	low_color;

	low_color = gray;
	if (info->is_checker)
		low_color = green;
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 135, gray, "4                        Checker planes");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 125, low_color, "                _____");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 135, low_color, "               |");
	if (info->is_checker)
		mlx_string_put(info->mlx, info->win, info->text_x,
			info->text_y + 135, gray, "                 ON");
	else
		mlx_string_put(info->mlx, info->win, info->text_x,
			info->text_y + 135, gray, "                 OFF");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 135, low_color, "                     |");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 135, low_color, "                _____");
}

void	print_info_block(t_info *info)
{
	unsigned int	gray;
	unsigned int	green;

	green = 0x32CD32;
	gray = 0xC0C0C0;
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y, gray, "miniRT. Controls:");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 15, gray, "Esc/Q          Exit");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 30, gray, "Mouse L        Camera and direction");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 45, gray, "Mouse R        Camera rotation");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 60, gray, "WASD           Movement");
	mlx_string_put(info->mlx, info->win, info->text_x,
		info->text_y + 75, gray, "Scroll         Up/Down");
	print_low_res(info, gray, green);
	print_full_res(info, gray, green);
	print_mirr_refl(info, gray, green);
	print_checker(info, gray, green);
	print_camera_coord(info, gray);
	print_camera_view(info, gray);
}
