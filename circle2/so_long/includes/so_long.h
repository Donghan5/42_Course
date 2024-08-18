/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:26:07 by donghank          #+#    #+#             */
/*   Updated: 2024/08/18 17:21:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

/*------------- INCLUDE -------------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <fcntl.h>
# include "struct.h"
# include "libft.h"
/*-------------- TILE BITE ------------*/
# define TILES		32

/*------------- CHARACTER -------------*/
# define IMG_HIGHT	32
# define IMG_WIDTH	32

/*------------- MAP -------------*/
# define WALL		'1'
# define EMPTY		'0'
# define COLLECTER	'C'
# define PLAYER		'P'
# define EXIT		'E'

/*------------- X_EVENT_KEYS -------------*/
# define X_EVENT_KEY_PRESS		2
# define X_EVENT_KEY_RELEASE	3
# define X_EVENT_KEY_EXIT		17

/*------------- KEY MAPPING FOR LINUX-------------*/
# ifdef __linux__
#  define OS	"linux"
#  include "../mlx_linux/mlx.h"
#  define KEY_W		119
#  define KEY_A		97
#  define KEY_S		115
#  define KEY_D		100
#  define KEY_ESC	65307
/*------------- KEY MAPPING FOR MACOS -------------*/
# elif __APPLE__
#  include "../mlx_mac/mlx.h"
#  define OS		"macos"
#  define KEY_W		13
#  define KEY_A		0
#  define KEY_S		1
#  define KEY_D		2
#  define KEY_ESC	53
# endif

/*------------- PATH_PLAYER.C -------------*/
void	free_visited(int **visited, int height);
int		**init_visited(t_game *game);
void	dfs(t_game *game, t_check *check, int y, int x);
void	init_check(t_game *game, t_check *check);
void	check_path(t_game *game);

/*------------- PLAYER_MOVE.C -------------*/
int		check_valid_move(t_game *game, int y, int x, int keysym);
int		check_player_move(int keysym, t_game *game);
void	pass_exit(t_game *game, int prev_y, int prev_x);
void	img_update_player(t_game *game, int prev_y, int prev_x);

/*------------- INIT.C -------------*/
void	init_mlx(t_game *game);
void	init_game_param(t_game *game);
void	init_map(t_game *game, int fd);
void	map_line_and_player(t_game *game, char *line, int len);

/*------------- DRAW_IMG.C -------------*/
void	put_image_pixel(t_game *game, char texture, int h, int w);
void	draw_map(t_game *game, char *line, int len);

/*------------- MAP.C -------------*/
void	set_map_val(t_game *game, char compo);
int		map_checking(t_game *game, char *line, int wall_check, int fd);
int		check_surrounded_wall(char *line);
int		generate_map(t_game *game, int fd);

/*------------- SO_LONG.C -------------*/
void	show_move(int c);
void	free_images(t_game *game);
int		close_game(t_game *game, int type);
int		key_press(int keysym, t_game *game);
int		check_map_compo(t_game *game);

/*------------- UTIL_MAP.C -------------*/
void	put_image_wall(t_game *game, char texture, int h, int w);
void	put_image_collec(t_game *game, char texture, int h, int w);
void	put_image_player(t_game *game, char texture, int h, int w);
void	put_image_empty(t_game *game, char texture, int h, int w);
void	put_image_exit(t_game *game, char texture, int h, int w);

/*------------- UTILS.C -------------*/
void	free_mlx_lib(t_game *game);
int		close_error(int err_type);
void	detect_os(t_game game);
void	clear_buffer(void);
int		check_arg_line(int ac, char **av);

#endif
