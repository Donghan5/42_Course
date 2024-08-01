/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:37:41 by donghank          #+#    #+#             */
/*   Updated: 2024/07/25 14:15:46 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* put the image of the wall */
void	put_image_wall(t_game *game, char texture, int h, int w)
{
	if (texture == WALL)
	{
		if (game->imgs.wall)
			mlx_destroy_image(game->mlx, game->imgs.wall);
		game->imgs.wall = mlx_xpm_file_to_image(game->mlx, \
					"imgs/wall.xpm", &w, &h);
		if (!game->imgs.wall)
			close_game(game, 0);
	}
}

/* put the image of the collecter */
void	put_image_collec(t_game *game, char texture, int h, int w)
{
	if (texture == COLLECTER && !game->imgs.collec)
	{
		game->imgs.collec = mlx_xpm_file_to_image(game->mlx, \
					"imgs/collectable.xpm", &w, &h);
		if (!game->imgs.collec)
			close_game(game, 0);
	}
}

/* put the image of the player */
void	put_image_player(t_game *game, char texture, int h, int w)
{
	if (texture == PLAYER)
	{
		if (game->imgs.player)
			mlx_destroy_image(game->mlx, game->imgs.player);
		game->imgs.player = mlx_xpm_file_to_image(game->mlx, \
					"imgs/play.xpm", &w, &h);
		if (!game->imgs.player)
			close_game(game, 0);
		game->position.img_ptr = game->imgs.player;
	}
}

/* put the image of the empty space */
void	put_image_empty(t_game *game, char texture, int h, int w)
{
	if (texture == EMPTY)
	{
		game->imgs.empty = mlx_xpm_file_to_image(game->mlx, \
					"imgs/background.xpm", &w, &h);
		if (!game->imgs.empty)
			close_game(game, 0);
	}
}

/* put the image of the exit */
void	put_image_exit(t_game *game, char texture, int h, int w)
{
	if (texture == EXIT)
	{
		game->imgs.exit = mlx_xpm_file_to_image(game->mlx, \
					"imgs/exit.xpm", &w, &h);
		if (!game->imgs.exit)
			close_game(game, 0);
	}
}
