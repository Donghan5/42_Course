/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:26:03 by donghank          #+#    #+#             */
/*   Updated: 2024/07/18 14:44:55 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

/*------------- IMAGES -------------*/
typedef struct s_img
{
	void	*wall;
	void	*exit;
	void	*collec;
	void	*player;
	void	*empty;
	int		*data;
	int		bpp;
	int		line_length;
	int		endian;
}	t_img;

/*------------- MAP ELEMENTS -------------*/
typedef struct s_map
{
	int		wall;
	int		player;
	int		collectiable;
	int		exit;
}	t_map;

/*------------- CHECK THE MAP VAL -------------*/
typedef struct s_check
{
	int	y;
	int	x;
	int	collectible;
	int	**visited;
}	t_check;

/*------------- PARAMETERS AXES X AND Y-------------*/
typedef struct s_param
{
	int		x;
	int		y;
	int		tile_x;
	int		tile_y;
	void	*img_ptr;
}	t_param;

/*------------- GAMES -------------*/
typedef struct s_game
{
	void	*mlx;
	void	*win;
	int		width;
	int		height;
	int		collect;
	int		move;
	char	**map;
	int		vaild_path;
	t_img	imgs;
	t_param	position;
	t_map	map_texture;
}	t_game;

#endif
