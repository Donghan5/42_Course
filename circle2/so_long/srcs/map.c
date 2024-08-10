/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:49 by donghank          #+#    #+#             */
/*   Updated: 2024/08/10 15:24:12 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* initial setting of the map */
void	set_map_val(t_game *game, char compo)
{
	if (compo == 'W' && !game->map_texture.wall)
		game->map_texture.wall = 1;
	else if (compo == 'C')
		game->map_texture.collectiable += 1;
	else if (compo == 'P')
	{
		if (game->map_texture.player > 0)
			close_error(5);
		game->map_texture.player = 1;
	}
	else if (compo == 'E')
	{
		if (game->map_texture.exit > 0)
			close_error(5);
		game->map_texture.exit = 1;
	}
}

/* checking the map is available */
int	map_checking(t_game *game, char *line, int wall_check)
{
	int		i;
	int		len;

	i = -1;
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		len -= 1;
	if (len != game->width)
		close_error(1);
	while (line[++i] && line[i] != '\n')
	{
		if (wall_check || i == 0 || i == len - 1)
		{
			if (line[i] != '1')
				close_error(3);
		}
		if (line[i] != '1' && line[i] != '0' && line[i] != 'P'
			&& line[i] != 'E' && line[i] != 'C')
			close_error(0);
		set_map_val(game, line[i]);
	}
	return (0);
}

/* checking the maps is surrounded by the wall */
int	check_surrounded_wall(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

/* generate map to show */
void	generate_map(t_game *game, int fd)
{
	char	*line;
	char	hei;

	hei = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (hei == 0)
		{
			game->width = ft_strlen(line) - 1;
			map_checking(game, line, 1);
		}
		else
			map_checking(game, line, 0);
		hei += 1;
		free(line);
	}
	game->height = hei;
	check_map_compo(game);
}
