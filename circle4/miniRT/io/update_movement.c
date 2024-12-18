/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:43:38 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/16 15:13:34 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	no_camera_conscious_moves(t_info *info,
	double forward, double sideways)
{
	if (forward > 0)
		info->scene.camera.coordinates.z++;
	else if (forward < 0)
		info->scene.camera.coordinates.z--;
	else if (sideways > 0)
		info->scene.camera.coordinates.x++;
	else if (sideways < 0)
		info->scene.camera.coordinates.x--;
}

static void	apply_movement(t_info *info, double forward, double sideways)
{
	double	horiz_cam;

	if (info->rotate_camera == 1)
		return (no_camera_conscious_moves(info, forward, sideways));
	horiz_cam = atan2(info->scene.camera.orientation.x,
			info->scene.camera.orientation.z);
	if (forward == 0 && sideways != 0)
		sideways *= 0.7071067811865476;
	if (forward != 0)
	{
		info->scene.camera.coordinates.x += forward * sin(horiz_cam);
		info->scene.camera.coordinates.z += forward * cos(horiz_cam);
	}
	if (sideways != 0)
	{
		info->scene.camera.coordinates.x += sideways
			* sin(horiz_cam + M_PI / 2);
		info->scene.camera.coordinates.z += sideways
			* cos(horiz_cam + M_PI / 2);
	}
}

static void	update_key_states(t_info *info, t_key_state *keys,
	int keycode, int is_pressed)
{
	if (is_pressed)
	{
		if (keycode == 65361 || keycode == 97)
			keys->left = 1;
		else if (keycode == 65363 || keycode == 100)
			keys->right = 1;
		else if (keycode == 65362 || keycode == 119)
			keys->forward = 1;
		else if (keycode == 65364 || keycode == 115)
			keys->backward = 1;
	}
	else
	{
		if (keycode == 65361 || keycode == 97)
			keys->left = 0;
		else if (keycode == 65363 || keycode == 100)
			keys->right = 0;
		else if (keycode == 65362 || keycode == 119)
			keys->forward = 0;
		else if (keycode == 65364 || keycode == 115)
			keys->backward = 0;
		if (!keys->left && !keys->backward && !keys->forward && !keys->right)
			info->is_input = 0;
	}
}

void	update_movement(t_info *info, int keycode, int is_pressed)
{
	double				forward;
	double				sideways;
	static t_key_state	keys;

	forward = 0;
	sideways = 0;
	update_key_states(info, &keys, keycode, is_pressed);
	if (keys.forward)
		forward += 2.0;
	if (keys.backward)
		forward -= 2.0;
	if (keys.right)
		sideways -= 2.0;
	if (keys.left)
		sideways += 2.0;
	if (forward != 0 || sideways != 0)
	{
		info->is_input = 1;
		apply_movement(info, forward, sideways);
	}
}
