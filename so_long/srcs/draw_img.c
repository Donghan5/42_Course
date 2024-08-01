/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_img.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:28 by donghank          #+#    #+#             */
/*   Updated: 2024/07/24 14:40:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* Draw the image or map in level of the pixel(32 bite)*/
void	put_image_pixel(t_game *game, char texture, int h, int w)
{
	if (texture == WALL)
	{
		put_image_wall(game, texture, h, w);
		mlx_put_image_to_window(game->mlx, game->win, game->imgs.wall, h, w);
	}
	if (texture == COLLECTER)
	{
		put_image_collec(game, texture, h, w);
		mlx_put_image_to_window(game->mlx, game->win, game->imgs.collec, h, w);
	}
	if (texture == PLAYER)
	{
		put_image_player(game, texture, h, w);
		mlx_put_image_to_window(game->mlx, game->win, game->imgs.player, h, w);
	}
	if (texture == EMPTY)
	{
		put_image_empty(game, texture, h, w);
		mlx_put_image_to_window(game->mlx, game->win, game->imgs.empty, h, w);
	}
	if (texture == EXIT)
	{
		put_image_exit(game, texture, h, w);
		mlx_put_image_to_window(game->mlx, game->win, game->imgs.exit, h, w);
	}
}

/* Drawing the map*/
void	draw_map(t_game *game, char *line, int len)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '0')
		{
			i++;
			continue ;
		}
		if (line[i] == PLAYER)
		{
			game->position.tile_x = i * TILES;
			game->position.tile_y = len * TILES;
		}
		put_image_pixel(game, line[i], i * TILES, len * TILES);
		i++;
	}
}
