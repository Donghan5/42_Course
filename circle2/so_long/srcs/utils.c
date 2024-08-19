/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:16:33 by donghank          #+#    #+#             */
/*   Updated: 2024/08/19 12:01:49 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* to treat the errors */
int	close_error(int err_type)
{
	ft_printf("\033[0;31m[ERROR]: \033[0m");
	if (err_type == -1)
		ft_printf("Fail to open the map.\n");
	else if (err_type == 0)
		ft_printf("The map have to include '0', '1', 'E', 'P'. 'C' \n");
	else if (err_type == 1)
		ft_printf("The map must be rectangular.\n");
	else if (err_type == 2)
		ft_printf("The argument must be <exec> <map_name.ber>. \n");
	else if (err_type == 3)
		ft_printf("The map is not surrounded by the wall. \n");
	else if (err_type == 4)
		ft_printf("The wall does not exist. \n");
	else if (err_type == 5)
		ft_printf("The player or the exit must be one. \n");
	else if (err_type == 6)
		ft_printf("Make sure of the name of the map.\n");
	else if (err_type == 7)
		ft_printf("Fail to allocate it. \n");
	exit(EXIT_FAILURE);
}

/* detect the os for mlx_hook*/
void	detect_os(t_game game)
{
	if (ft_strncmp(OS, "linux", 5) == 0)
	{
		mlx_hook(game.win, X_EVENT_KEY_PRESS, 1L << 0, &key_press, &game);
		mlx_hook(game.win, X_EVENT_KEY_EXIT, 1L << 0, &close_game, &game);
	}
	if (ft_strncmp(OS, "macos", 5) == 0)
	{
		mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &key_press, &game);
		mlx_hook(game.win, X_EVENT_KEY_EXIT, 0, &close_game, &game);
	}
}

/* free mlx lib resources */
void	free_mlx_lib(t_game *game)
{
	free_images(game);
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

/* check arguemet line is valid */
int	check_arg_line(int ac, char **av)
{
	if (ac != 2)
		return (1);
	if (ft_strncmp(av[1] + ft_strlen(av[1]) - 4, ".ber", 4) != 0)
		return (1);
	return (0);
}
