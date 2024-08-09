/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:29:35 by donghank          #+#    #+#             */
/*   Updated: 2024/08/09 17:13:40 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	read_files(int fd, char **buffer)
{
	char	*tmp;
	ssize_t	byte;
	char	*tmp_buffer;

	tmp = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!tmp)
		return (-1);
	byte = read(fd, tmp, BUFFER_SIZE);
	while (byte > 0)
	{
		tmp[byte] = '\0';
		if (!*buffer)
			*buffer = ft_strdup(tmp);
		else
		{
			tmp_buffer = ft_strjoin(*buffer, tmp);
			free(*buffer);
			*buffer = tmp_buffer;
		}
		if (ft_strchr(*buffer, '\n') != NULL)
			break ;
		byte = read(fd, tmp, BUFFER_SIZE);
	}
	free(tmp);
	return (byte);
}

static void	get_free(char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
}

static char	*get_line(char **buffer)
{
	char	*line;
	char	*line_pos;
	char	*tmp_buffer;
	size_t	len;

	line_pos = ft_strchr(*buffer, '\n');
	if (line_pos)
	{
		len = line_pos - *buffer + 1;
		line = ft_substr(*buffer, 0, len);
		tmp_buffer = ft_strdup(line_pos + 1);
		free(*buffer);
		*buffer = tmp_buffer;
		if (*buffer && (*buffer)[0] == '\0')
			get_free(buffer);
	}
	else
	{
		line = ft_strdup(*buffer);
		get_free(buffer);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	ssize_t		byte;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	byte = read_files(fd, &buffer);
	if (byte == -1)
		return (get_free(&buffer), NULL);
	else if (byte == 0 && (!buffer || !*buffer))
		return (get_free(&buffer), NULL);
	return (get_line(&buffer));
}
