/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_actions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:48:58 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/15 18:26:54 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	simple_key_actions(int keycode, t_info *info)
{
	if (keycode == 49 && info->toggle_mode != TOGGLE_LOW)
	{
		info->render_type = LOW_RENDER;
		info->toggle_mode = TOGGLE_LOW;
	}
	else if (keycode == 50 && info->toggle_mode != TOGGLE_FULL)
	{
		info->render_type = FULL_RENDER;
		info->toggle_mode = TOGGLE_FULL;
	}
	else if (keycode == 49)
	{
		info->render_type = FULL_RENDER;
		info->toggle_mode = TOGGLE_OFF;
	}
	else if (keycode == 50)
	{
		info->render_type = LOW_RENDER;
		info->toggle_mode = TOGGLE_OFF;
	}
}

int	key_pressed(int keycode, t_info *info)
{
	if (keycode == 65307 || keycode == 113)
		close_window(info);
	else
	{
		simple_key_actions(keycode, info);
		if (keycode == 51 && info->is_mirror == 1)
			info->is_mirror = 0;
		else if (keycode == 51)
			info->is_mirror = 1;
		if (keycode == 51)
			info->render_type = LOW_RENDER;
		if (keycode == 52 && info->is_checker == 1)
			info->is_checker = 0;
		else if (keycode == 52)
			info->is_checker = 1;
		if (keycode == 52)
			info->render_type = LOW_RENDER;
		update_movement(info, keycode, 1);
	}
	return (0);
}

int	key_off(int keycode, t_info *info)
{
	update_movement(info, keycode, 0);
	return (0);
}
