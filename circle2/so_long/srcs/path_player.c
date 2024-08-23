/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:52 by donghank          #+#    #+#             */
/*   Updated: 2024/08/23 18:07:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/*
	Free the array visited
	i = index
*/
void	free_visited(int **visited, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}

/*
	Set the array visited
 	visited = track
 	i = height
 	j = width
*/
int	**init_visited(t_game *game)
{
	int	**visited;
	int	i;
	int	j;

	if (game->height <= 0 || game->width <= 0)
		return (0);
	i = -1;
	visited = (int **)malloc(sizeof(int *) * game->height);
	if (!visited)
		return (0);
	while (++i < game->height)
	{
		visited[i] = (int *)malloc(sizeof(int) * game->width);
		if (!visited[i])
			return (free_visited(visited, i), NULL);
		j = 0;
		while (j < game->width)
		{
			visited[i][j] = 0;
			j++;
		}
	}
	return (visited);
}

/*
	Using deep first search algo
	dir_y = direction y axe
	dir_x = dirction x axe
	next_y = next y axe
	next_x = next x axe
	i = index
*/
void	dfs(t_game *game, t_check *check, int y, int x)
{
	const int	dir_y[4] = {1, -1, 0, 0};
	const int	dir_x[4] = {0, 0, 1, -1};
	int			next_y;
	int			next_x;
	int			i;

	check->visited[y][x] = 1;
	if (game->map[y][x] == COLLECTER)
		check->collectible -= 1;
	if (game->map[y][x] == EXIT)
	{
		game->vaild_path = 1;
		return ;
	}
	i = -1;
	while (++i < 4)
	{
		next_y = y + dir_y[i];
		next_x = x + dir_x[i];
		if (game->map[next_y][next_x] != '1' && \
		!check->visited[next_y][next_x] && \
		next_y >= 0 && next_y < game->height && \
		next_x >= 0 && next_x < game->width)
			dfs(game, check, next_y, next_x);
	}
}

/* setting the element of the struct check */
void	init_check(t_game *game, t_check *check)
{
	check->visited = init_visited(game);
	check->y = game->height;
	check->x = game->width;
	check->collectible = game->map_texture.collectiable;
}

/* checking the path is validated */
void	check_path(t_game *game)
{
	t_check	check;

	init_check(game, &check);
	dfs(game, &check, game->position.y, game->position.x);
	if (!game->vaild_path || check.collectible > 0)
	{
		free_visited(check.visited, game->height);
		free_mlx_lib(&(*game));
		ft_printf("\033[1;31m[ERROR]: \033[0m");
		ft_printf("The map formule error\n");
		close_game(game, 0);
	}
	free_visited(check.visited, game->height);
}
