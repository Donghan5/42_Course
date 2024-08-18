/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:25:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/18 17:17:09 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	main(int ac, char **av)
{
	t_game	game;
	int		fd;

	if (check_arg_line(ac, av) == 1)
		return (ft_printf("[ERROR]: <exec> <mapname.ber>\n"), 1);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		return (ft_printf("[ERROR]: Fail open file\n"), 1);
	init_game_param(&game);
	if (generate_map(&game, fd) == 1)
		return (close(fd), 1);
	init_mlx(&game);
	if (!game.mlx || !game.win)
		return (ft_printf("[ERROR]: Fail mlx init\n"), free_mlx_lib(&game), 1);
	init_map(&game, fd);
	close(fd);
	detect_os(game);
	mlx_loop(game.mlx);
	free_mlx_lib(&game);
	return (0);
}
