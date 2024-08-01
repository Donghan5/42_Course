/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:55 by donghank          #+#    #+#             */
/*   Updated: 2024/07/18 15:03:56 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* to check the moving possiblitiy */
int	check_valid_move(t_game *game, int y, int x, int keysym)
{
	char	cur;

	cur = game->map[y][x];
	if (keysym != KEY_A && keysym != KEY_D && \
		keysym != KEY_W && keysym != KEY_S)
		return (0);
	if (cur == WALL)
		return (0);
	else if (cur == COLLECTER)
	{
		game->collect += 1;
		game->map[y][x] = EMPTY;
	}
	else if (cur == EXIT)
	{
		if (game->collect == game->map_texture.collectiable)
			close_game(game, 1);
		else
			ft_printf("Remain the collectiables. \n");
	}
	return (1);
}

/* to check the move of the player */
int	check_player_move(int keysym, t_game *game)
{
	int	next_x;
	int	next_y;
	int	flag;

	flag = 1;
	next_y = game->position.y;
	next_x = game->position.x;
	if (keysym == KEY_W)
		next_y--;
	else if (keysym == KEY_S)
		next_y++;
	else if (keysym == KEY_A)
		next_x--;
	else if (keysym == KEY_D)
		next_x++;
	else
		flag = 0;
	flag = check_valid_move(game, next_y, next_x, keysym);
	if (!flag)
		return (flag);
	if (game->map[game->position.y][game->position.x] == EXIT)
		flag = 2;
	game->position.y = next_y;
	game->position.x = next_x;
	return (flag);
}

void	pass_exit(t_game *game, int prev_y, int prev_x)
{
	put_image_pixel(game, EXIT, prev_x, prev_y);
}

void	img_update_player(t_game *game, int prev_y, int prev_x)
{
	int	hei;
	int	wid;

	game->imgs.data = (int *)mlx_get_data_addr(game->position.img_ptr, \
				&game->imgs.bpp, &game->imgs.line_length, &game->imgs.endian);
	hei = 0;
	while (hei < TILES)
	{
		wid = 0;
		while (wid < TILES)
		{
			if (hei * TILES + wid < game->imgs.line_length / 4 * TILES)
				game->imgs.data[hei * TILES + wid] = 0;
			wid++;
		}
		hei++;
	}
	mlx_put_image_to_window(game->mlx, game->win, \
		game->position.img_ptr, prev_x, prev_y);
	put_image_pixel(game, PLAYER, game->position.tile_x, game->position.tile_y);
	mlx_put_image_to_window(game->mlx, game->win, \
		game->position.img_ptr, game->position.tile_x, game->position.tile_y);
	game->move += 1;
}
