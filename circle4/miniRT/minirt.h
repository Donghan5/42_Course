/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:40:16 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/17 14:50:54 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# include "minilibx-linux/mlx.h"
# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <float.h>
# define M_PI 3.14159265358979323846
# define SCREEN_HEIGHT 700
# define SCREEN_WIDTH 900
# define MAX_DEPTH 3
# define EPSILON 0.0001
# define PARSE_ERR "Fail to parse"
# define RGB "Fail to parse RGB"
# define COOR_ERR "Fail to coordinate parse"
# define ORI_ERR "Fail to orientation parse"

typedef char	t_bool;

typedef struct s_indices
{
	int	sp_idx;
	int	pl_idx;
	int	cy_idx;
	int	l_idx;
	int	co_idx;
	int	fd;
}				t_indices;

typedef struct s_vec3 {
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct s_color {
	int	r;
	int	g;
	int	b;
}				t_color;

typedef struct s_ray {
	t_vec3	origin;
	t_vec3	direction;
}				t_ray;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_ambient
{
	double	a_ratio;
	t_color	color;
}				t_ambient;

typedef struct s_camera
{
	t_vec3	coordinates;
	t_vec3	orientation;
	int		c_view;
}				t_camera;

typedef struct s_light
{
	t_vec3	coordinates;
	t_color	color;
	double	l_brightness;
}				t_light;

typedef struct s_sphere
{
	t_vec3	coordinates;
	t_color	color;
	double	sp_diameter;
}				t_sphere;

typedef struct s_plane
{
	t_vec3	coordinates;
	t_vec3	normal_vector;
	t_color	color;
}				t_plane;

typedef struct s_cylinder
{
	t_vec3	coordinates;
	t_vec3	axis_vector;
	t_color	color;
	double	cy_diameter;
	double	cy_height;
}				t_cylinder;

typedef struct s_cone
{
	t_vec3	coordinates;
	t_vec3	axis_vector;
	t_color	color;
	double	co_diameter;
	double	co_height;
}				t_cone;

typedef struct s_hit_material
{
	t_vec3	hit_point;
	t_vec3	normal;
	double	t;
	int		type;
	int		index;
	t_color	base_color;
	double	shininess;
	double	spec_intensity;
	double	diffuse_strength;
	double	reflectivity;
}				t_hit_material;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		**light;
	t_sphere	**sphere;
	t_plane		**plane;
	t_cylinder	**cylinder;
	t_cone		**cone;
	int			cone_n;
	int			sphere_n;
	int			plane_n;
	int			cylinder_n;
	int			light_n;
}				t_scene;

enum e_render_type
{
	NO_RENDER,
	LOW_RENDER,
	FULL_RENDER,
	TOGGLE_OFF,
	TOGGLE_LOW,
	TOGGLE_FULL
};

typedef struct s_key_state
{
	int	forward;
	int	backward;
	int	left;
	int	right;
}				t_key_state;

/*
	Rotating camera modes:
		0 - no rotation
		1 - just rotation
		2 - rotation with direction of movement change
*/
typedef struct s_info
{
	t_data	*img;
	t_scene	scene;
	void	*mlx;
	void	*win;
	int		rotate_camera;
	int		moving_text;
	int		moving_origin_x;
	int		moving_origin_y;
	int		text_x;
	int		text_y;
	t_bool	is_input;
	long	last_scroll_time;
	int		render_type;
	int		toggle_mode;
	t_bool	is_mirror;
	t_bool	is_checker;
}				t_info;

typedef struct s_count
{
	int	a_count;
	int	c_count;
	int	l_count;
	int	sp_count;
	int	pl_count;
	int	cy_count;
	int	co_count;
}				t_count;

typedef struct s_intersection
{
	t_vec3	axis;
	t_vec3	center;
	double	height;
	double	half_height;
	t_vec3	v;
	t_vec3	p;
	double	d2;
	double	radius;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;
	double	t_cap;
	double	denom;
}				t_intersection;

//	calc_intersections
//		intersect_cylinder.c
double	intersect_cylinder(t_ray ray, t_cylinder *cyl);
//		intersect_sphere.c
double	intersect_sphere(t_ray ray, t_sphere *sphere);
//		intersect_plane.c
double	intersect_plane(t_ray ray, t_plane *plane);
//		intersect_cone.c
double	intersect_cone(t_ray ray, t_cone *co);

//	calc_light
//		calc_diffuse_spec.c
t_color	multiply_colors(t_color a, t_color b, double factor);
t_color	add_white_component(t_color base, double factor);
void	calc_diffuse_spec(t_hit_material closest, t_info *info, t_ray ray,
			t_color (*total_diff_spec)[2]);
//		check_shadow.c
double	check_shadow(t_info *info, t_vec3 hit_point, t_vec3 light_pos);

//	info_block
//		info_block.c
void	print_mirr_refl(t_info *info, unsigned int gray, unsigned int green);
void	print_low_res(t_info *info, unsigned int gray, unsigned int green);
void	print_full_res(t_info *info, unsigned int gray, unsigned int green);
void	print_camera_coord(t_info *info, unsigned int gray);
void	print_camera_view(t_info *info, unsigned int gray);
//		print_info_block.c
void	print_info_block(t_info *info);

//	io
//		key_actions.c
int		key_pressed(int keycode, t_info *info);
int		key_off(int keycode, t_info *info);
//		mouse_actions.c
int		mouse_clicks(int keycode, int x, int y, t_info *info);
int		mouse_off(int keycode, int x, int y, t_info *info);
int		mouse_moves(int x, int y, t_info *info);
//		update_movement.c
void	update_movement(t_info *info, int keycode, int is_pressed);

//	parsing
//		check.c
void	check_int(int fd, char *map_info, size_t *i);
void	check_doub(int fd, char *map_info, size_t *i);
void	check_three(int fd, char *map_info, size_t *i, char *type);
void	check_validity(char *map_info, int fd);
//		free.c
void	free_doub_array(char **strs);
//		init_shape.c
void	init_light(t_scene *scene);
void	init_cone(t_scene *scene);
void	init_sphere(t_scene *scene);
void	init_plane(t_scene *scene);
void	init_cylinder(t_scene *scene);
//		init.c
void	init_scene(char *path, t_scene *scene);
void	init_ambient(t_scene *scene);
void	init_camera(t_scene *scene);
void	init_indices(t_indices *indices);
void	init_count(t_count *count);
//		parse_tool.c
t_bool	is_rt_extension(char *map_name);
void	count_objs(char *path, t_scene *scene);
//		parse_utils.c
void	handle_error(char *msg);
int		ft_isspace(char c);
int		is_empty_or_comment(char *line);
//		parsing.c
void	parse_scene(char *path, t_scene *scene);
//		process_parse.c
void	handle_error_free(char *map_line, char *msg);
void	process_parse(char *map_line, t_scene *scene, t_indices *indices);
//		stock_ambient.c
void	stock_ambient(int fd, t_scene *scene, char *info_map);
//		stock_basic.c
t_bool	stock_rgb(t_color *color, char **rgb_infos);
void	stock_infos(int fd, int type, t_scene *scene, char *info_map);
//		stock_cam.c
void	stock_cam(int fd, t_scene *scene, char *info_map);
//		stock_cone.c
void	stock_cone(t_scene *scene, char *info_map, t_indices *indices);
//		stock_cylinder.c
void	stock_cylinder(t_scene *scene, char *info_map, t_indices *indices);
//		stock_light.c
void	stock_light(t_scene *scene, char *info_map, t_indices *indices);
//		stock_plane.c
void	stock_plane(t_scene *scene, char *info_map, t_indices *indices);
//		stock_sphere.c
void	stock_sphere(t_scene *scene, char *info_map, t_indices *indices);
//		type.c
int		get_type(char *map_info);
//		valid.c
void	validity_type_one(char *map_info, size_t *i, int fd);
void	validity_type_two(char *map_info, size_t *i, int fd);
void	validity_type_three(char *map_info, size_t *i, int fd);
void	validity_type_four(char *map_info, size_t *i, int fd);
void	validity_type_five(char *map_info, size_t *i, int fd);

//	render
//		fill_material.c
void	fill_material(t_hit_material *closest, t_info *info);
//		frame_render.c
int		render_next_frame(t_info *info);
//		get_checker_color.c
t_color	get_checker_color(t_vec3 hit_point,
			t_vec3 normal, t_color base_color);
//		render_scene.c
void	render_scene(t_info *info);
int		close_window(t_info *info);
void	handle_map_check(char *map_line, t_scene *scene, t_indices *indices);
//		trace_ray.c
t_color	trace_ray(t_ray ray, t_info *info, int depth);

//	tools
//		extra_operations.c
t_color	t_color_add(t_color a, t_color b);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
//		free_tools.c
void	exit_error(int fd, char *line, t_scene *scene, char *message);
void	smart_free(void *element);
void	free_scene_safe(t_scene *scene);
//		ft_atod.c
double	ft_atod(const char *str);
//		tools.c
t_color	normalize_color(t_color color);
int		is_equal(double a, double b);
void	ft_putstr_fd(char *s, int fd);
//		vector_operations.c
t_vec3	vec3(double x, double y, double z);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_mul(t_vec3 v, double t);
t_vec3	vec3_hadamard(t_vec3 a, t_vec3 b);

#endif
