/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:39:54 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/17 13:48:44 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	close_window(t_info *info)
{
	mlx_destroy_window(info->mlx, info->win);
	mlx_destroy_image(info->mlx, info->img->img);
	mlx_destroy_display(info->mlx);
	free_scene_safe(&info->scene);
	return (free(info->mlx), exit (0), 0);
}

void	install_hooks(t_info *info)
{
	mlx_hook(info->win, 17, 0, close_window, info);
	mlx_hook(info->win, 2, 1L << 0, key_pressed, info);
	mlx_hook(info->win, 3, 1L << 1, key_off, info);
	mlx_hook(info->win, 4, 1L << 2, mouse_clicks, info);
	mlx_hook(info->win, 5, 1L << 3, mouse_off, info);
	mlx_hook(info->win, 6, 1L << 13, mouse_moves, info);
}

void	info_initializer(t_info *info)
{
	info->rotate_camera = 0;
	info->moving_text = 0;
	info->text_x = SCREEN_WIDTH - 300;
	info->text_y = SCREEN_HEIGHT - 250;
	info->is_input = 0;
	info->last_scroll_time = 0;
	info->is_mirror = 0;
	info->is_checker = 0;
	info->render_type = LOW_RENDER;
	info->toggle_mode = TOGGLE_LOW;
}

int	main(int argc, char **argv)
{
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	t_info	info;

	if (argc != 2 || !is_rt_extension(argv[1]))
		exit_error(0, NULL, NULL,
			"Wrong arguments. Format: ./miniRT \"path_to_rt_scene.rt\"");
	parse_scene(argv[1], &info.scene);
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT");
	img.img = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	img.addr = mlx_get_data_addr(img.img,
			&img.bits_per_pixel, &img.line_length, &img.endian);
	info.img = &img;
	info.mlx = mlx;
	info.win = mlx_win;
	info_initializer(&info);
	install_hooks(&info);
	mlx_loop_hook(info.mlx, render_next_frame, &info);
	mlx_loop(info.mlx);
	close_window(&info);
	return (exit(0), 0);
}
