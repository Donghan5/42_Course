/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:29:35 by donghank          #+#    #+#             */
/*   Updated: 2024/08/19 16:10:01 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	read_files(int fd, char **buffer)
{
	char	tmp[BUFFER_SIZE + 1];
	ssize_t	byte;
	char	*tmp_buffer;

	byte = read(fd, tmp, BUFFER_SIZE);
	while (byte > 0)
	{
		tmp[byte] = '\0';
		if (!buffer[fd])
			buffer[fd] = ft_strdup(tmp);
		else
		{
			tmp_buffer = ft_strjoin(buffer[fd], tmp);
			free(buffer[fd]);
			buffer[fd] = tmp_buffer;
		}
		if (ft_strchr(buffer[fd], '\n') != NULL)
			break ;
		byte = read(fd, tmp, BUFFER_SIZE);
	}
	return (byte);
}

static void	get_free(int fd, char **buffer)
{
	if (buffer[fd])
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
}

static char	*get_line(int fd, char **buffer)
{
	char	*line;
	char	*line_pos;
	char	*tmp_buffer;

	line_pos = ft_strchr(buffer[fd], '\n');
	if (line_pos)
	{
		line = ft_substr(buffer[fd], 0, line_pos - buffer[fd] + 1);
		if (!line)
			return (get_free(fd, buffer), NULL);
		tmp_buffer = ft_strdup(line_pos + 1);
		if (!tmp_buffer)
			return (free(line), get_free(fd, buffer), NULL);
		free(buffer[fd]);
		buffer[fd] = tmp_buffer;
		if (buffer[fd] && buffer[fd][0] == '\0')
			get_free(fd, buffer);
	}
	else
	{
		line = ft_strdup(buffer[fd]);
		get_free(fd, buffer);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer[8192];
	ssize_t		byte;

	if (fd < 0 || 8192 <= fd || BUFFER_SIZE <= 0)
		return (NULL);
	byte = read_files(fd, buffer);
	if (byte == -1)
	{
		get_free(fd, buffer);
		return (NULL);
	}
	else if (byte == 0 && (!buffer[fd] || !*buffer[fd]))
	{
		get_free(fd, buffer);
		return (NULL);
	}
	return (get_line(fd, buffer));
}
