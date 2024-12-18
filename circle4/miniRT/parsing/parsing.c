/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:39:31 by pzinurov          #+#    #+#             */
/*   Updated: 2024/12/17 14:29:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	change multiple white spaces to single white space
	@param
		line: line which want to switch
	@return
		new_start: converted line (i.e. convert mult space to single)
*/
char	*normalize_line(char **line)
{
	char	*new_line;
	char	*new_start;
	char	*line_start;

	line_start = *line;
	new_line = malloc(ft_strlen(*line) + 2);
	if (!new_line)
		return (smart_free(*line), *line = NULL, NULL);
	new_start = new_line;
	while (**line)
	{
		while (**line && !ft_isspace(**line))
			*new_line++ = *(*line)++;
		if (!ft_isspace(**line))
			continue ;
		*new_line = ' ';
		while (ft_isspace(**line))
			(*line)++;
		if (**line != ',' && new_line != new_start && *(new_line - 1) != ',')
			new_line++;
	}
	*new_line = '\0';
	smart_free(line_start);
	*line = new_start;
	return (new_start);
}

/*
	to stock the all element in the .rt maps
	@param
		fd: file discriptor of the map
		scene: structure which want to stock it
*/
static t_bool	do_stock(int fd, t_scene *scene)
{
	char		*map_line;
	t_indices	indices;

	init_indices(&indices);
	indices.fd = fd;
	map_line = get_next_line(fd);
	if (!map_line)
		return (0);
	while (map_line != NULL)
	{
		if (!is_empty_or_comment(map_line))
		{
			normalize_line(&map_line);
			if (!map_line)
				return (0);
			process_parse(map_line, scene, &indices);
		}
		free(map_line);
		map_line = get_next_line(fd);
	}
	return (1);
}

/*
	To check the validity of the line
	@param
		fd: file descriptor of the map
*/
static t_bool	validate(int fd)
{
	char	*map_line;

	map_line = get_next_line(fd);
	if (!map_line)
		return (0);
	while (map_line != NULL)
	{
		if (!is_empty_or_comment(map_line))
			check_validity(map_line, fd);
		free(map_line);
		map_line = get_next_line(fd);
	}
	return (1);
}

/*
	parse and stock all informations
	<general logic>
	1. parsing the map_line (read by gnl) by separating white-space
	2. verify element (map_infos[0] is char) the other have just comma(,) and num
	3. separate the case --> stock each structures
	@param
		path: path of the file
		scene: struct information
		type: type of information
		fd: file descriptor of path (map with .rt extension)
*/
void	parse_scene(char *path, t_scene *scene)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0 || !validate(fd))
	{
		close(fd);
		handle_error("Fail to open the file");
	}
	close(fd);
	init_scene(path, scene);
	fd = open(path, O_RDONLY);
	if (fd < 0 || !do_stock(fd, scene))
		exit_error(0, NULL, scene, "Fail to open the file");
	close(fd);
}
