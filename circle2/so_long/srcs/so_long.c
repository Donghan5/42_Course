/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:58 by donghank          #+#    #+#             */
/*   Updated: 2024/08/14 12:43:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
**	key mapping
**	prev_x and prev_y = before axe x and y
*/
int	key_press(int keysym, t_game *game)
{
	int		prev_x;
	int		prev_y;
	int		flag;

	if (keysym == KEY_ESC)
		close_game(game, 1);
	flag = check_player_move(keysym, game);
	if (flag)
	{
		prev_x = game->position.tile_x;
		prev_y = game->position.tile_y;
		if (keysym == KEY_W)
			game->position.tile_y -= TILES;
		else if (keysym == KEY_S)
			game->position.tile_y += TILES;
		else if (keysym == KEY_A)
			game->position.tile_x -= TILES;
		else if (keysym == KEY_D)
			game->position.tile_x += TILES;
		img_update_player(game, prev_y, prev_x);
		if (flag == 2)
			pass_exit(game, prev_y, prev_x);
		show_move(game->move);
	}
	return (0);
}

/* check the elements map */
void	check_map_compo(t_game *game, int fd)
{
	if (!game->map_texture.exit)
	{
		close(fd);
		close_error(0);
	}
	if (!game->map_texture.player)
	{
		close(fd);
		close_error(0);
	}
	if (!game->map_texture.collectiable)
	{
		close(fd);
		close_error(0);
	}
}

/* To show the current move*/
void	show_move(int c)
{
	ft_printf("\033[0;32m[CURRENT MOVE]: \033[0m");
	ft_printf("\033[0;32m%d\033[0m\n", c);
}

/* free all of the images */
void	free_images(t_game *game)
{
	if (game->imgs.wall)
	{
		mlx_destroy_image(game->mlx, game->imgs.wall);
		game->imgs.wall = NULL;
	}
	if (game->imgs.collec)
	{
		mlx_destroy_image(game->mlx, game->imgs.collec);
		game->imgs.collec = NULL;
	}
	if (game->imgs.player)
	{
		mlx_destroy_image(game->mlx, game->imgs.player);
		game->imgs.player = NULL;
	}
	if (game->imgs.empty)
	{
		mlx_destroy_image(game->mlx, game->imgs.empty);
		game->imgs.empty = NULL;
	}
	if (game->imgs.exit)
	{
		mlx_destroy_image(game->mlx, game->imgs.exit);
		game->imgs.exit = NULL;
	}
}

/* Close the games */
int	close_game(t_game *game, int type)
{
	int	i;

	free_images(game);
	i = 0;
	while (i < game->height)
	{
		free(game->map[i]);
		i++;
	}
	free(game->map);
	free_mlx_lib(game);
	if (type)
		ft_printf("\033[0;31m[GAME OVER]\033[0m\n");
	exit(0);
}
