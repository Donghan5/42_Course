/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:34 by donghank          #+#    #+#             */
/*   Updated: 2024/08/09 16:58:10 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* initialize all element concern of the map */
void	init_mlx(t_game *game)
{
	int	w;
	int	h;

	w = game->width;
	h = game->height;
	game->mlx = mlx_init();
	if (!game->mlx)
		return ;
	game->win = mlx_new_window(game->mlx, w * TILES, h * TILES, "so_long");
	if (!game->win)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		return ;
	}
	game->imgs.wall = NULL;
	game->imgs.collec = NULL;
	game->imgs.player = NULL;
	game->imgs.exit = NULL;
	game->imgs.empty = NULL;
}

/* initialize all element concern of the game parameters */
void	init_game_param(t_game *game)
{
	game->width = 0;
	game->height = 0;
	game->collect = 0;
	game->map_texture.collectiable = 0;
	game->map_texture.exit = 0;
	game->map_texture.player = 0;
	game->map_texture.wall = 0;
	game->move = 0;
	game->map = NULL;
	game->position.x = 0;
	game->position.y = 0;
	game->vaild_path = 0;
}

/* initialize the map */
void	init_map(t_game *game, int fd)
{
	int		i;
	int		is_valid;
	char	*line;

	i = 0;
	is_valid = 1;
	game->map = (char **)malloc(sizeof(char *) * game->height);
	if (!game->map)
		return ;
	while (i < game->height)
	{
		line = get_next_line(fd);
		if (!line)
			close_game(game, 0);
		gen_map_col(game, line, i);
		if (i + 1 == game->height)
			is_valid = check_lastline(line);
		if (!is_valid)
			close_game(game, 0);
		draw_map(game, line, i);
		free(line);
		i++;
	}
	check_path(game);
	ft_printf("[GAME START]\n");
}

/* to make map line and update the position of the player */
void	gen_map_col(t_game *game, char *line, int len)
{
	int	i;

	i = 0;
	game->map[len] = (char *)malloc(sizeof(char) * (game->width + 1));
	if (!game->map[len])
		return ;
	while (i < game->width)
	{
		if (line[i] == PLAYER)
		{
			game->position.y = len;
			game->position.x = i;
		}
		game->map[len][i] = line[i];
		i++;
	}
	game->map[len][game->width] = '\0';
}
