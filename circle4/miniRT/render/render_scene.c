/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:51:26 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/13 15:06:06 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_vec3	calc_ray_dir(t_vec3 target, double px, double py)
{
	t_vec3	dir;
	t_vec3	right;
	t_vec3	up;
	t_vec3	forward;

	dir = vec3_normalize(vec3(1.0, py, px));
	if (target.x != 0 || target.y != 0 || target.z != 0)
	{
		right = vec3_normalize(vec3_cross(target, (t_vec3){0, 1, 0}));
		up = vec3_normalize(vec3_cross(right, target));
		forward = target;
		dir = vec3_normalize(vec3(
					dir.x * forward.x + dir.y * up.x + dir.z * right.x,
					dir.x * forward.y + dir.y * up.y + dir.z * right.y,
					dir.x * forward.z + dir.y * up.z + dir.z * right.z
					));
	}
	return (dir);
}

static t_ray	get_ray(int x, int y, t_info *info)
{
	t_ray		ray;
	t_vec3		target;
	double		fov;
	double		px;
	double		py;

	ray.origin = info->scene.camera.coordinates;
	target = vec3_normalize(info->scene.camera.orientation);
	fov = info->scene.camera.c_view * M_PI / 180.0;
	px = (2.0 * ((double)x / SCREEN_WIDTH) - 1.0) * tan(fov / 2.0);
	py = -(2.0 * ((double)y / SCREEN_HEIGHT) - 1.0)
		* tan(fov / 2.0) * SCREEN_HEIGHT / SCREEN_WIDTH;
	ray.direction = calc_ray_dir(target, px, py);
	return (ray);
}

static void	put_pixels_to_image(t_info *info, int y, int x, t_color pixel_color)
{
	unsigned int	color;
	int				i;
	int				j;
	int				scale;
	char			*dst;

	i = 0;
	scale = 1;
	if (info->toggle_mode != TOGGLE_FULL && info->render_type == LOW_RENDER)
		scale = 8;
	color = (pixel_color.r << 16) | (pixel_color.g << 8) | pixel_color.b;
	while (i < scale && (y + i) < SCREEN_HEIGHT)
	{
		j = 0;
		while (j < scale && (x + j) < SCREEN_WIDTH)
		{
			dst = info->img->addr + ((y + i) * info->img->line_length
					+ (x + j) * (info->img->bits_per_pixel / 8));
			*(unsigned int *)dst = color;
			j++;
		}
		i++;
	}
}

void	render_scene(t_info *info)
{
	int		scale;
	int		y;
	int		x;
	t_ray	ray;

	scale = 1;
	if (info->toggle_mode != TOGGLE_FULL && info->render_type == LOW_RENDER)
		scale = 8;
	y = 0;
	while (y < SCREEN_HEIGHT)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
		{
			ray = get_ray(x + 0.5, y + 0.5, info);
			put_pixels_to_image(info, y, x,
				normalize_color(trace_ray(ray, info, 0)));
			x += scale;
		}
		y += scale;
	}
	if (info->render_type == LOW_RENDER && info->toggle_mode == TOGGLE_OFF)
		info->render_type = FULL_RENDER;
	else
		info->render_type = NO_RENDER;
}
