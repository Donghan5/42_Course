/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 22:30:30 by pzinurov          #+#    #+#             */
/*   Updated: 2024/11/07 21:06:51 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

long long	next_c_index(char line[BUFFER_SIZE])
{
	long long	i;

	i = 0;
	while (i < BUFFER_SIZE)
	{
		if (line[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*move_left(char *buf)
{
	long long	i;
	char		*new_line;
	long long	nl_index;

	i = 0;
	nl_index = next_c_index(buf);
	if (nl_index < 0)
		nl_index = BUFFER_SIZE - 1;
	new_line = malloc(nl_index + 2);
	if (new_line == NULL)
		return (NULL);
	while (i <= nl_index)
	{
		new_line[i] = buf[i];
		i++;
	}
	new_line[i] = '\0';
	if (buf[nl_index + 1])
	{
		ft_memmove(buf, buf + i, BUFFER_SIZE - i);
		ft_bzero(buf + (BUFFER_SIZE - i), i + 1);
	}
	else
		ft_bzero(buf, BUFFER_SIZE);
	return (new_line);
}

char	*join_free(char *s1, char *s2)
{
	char	*new_s;

	if (s1 == NULL || s2 == NULL)
	{
		if (s2 != NULL)
			free (s2);
		if (s1 != NULL)
			free (s1);
		return (NULL);
	}
	new_s = ft_strjoin(s1, s2);
	free (s2);
	free (s1);
	return (new_s);
}

char	*join_until_nl(char *buf,
	int fd, int ret)
{
	long long	end_index;
	char		*line;

	line = malloc(1);
	if (line == NULL)
		return (NULL);
	line[0] = '\0';
	end_index = next_c_index(buf);
	while (end_index < 0)
	{
		line = join_free(line, move_left(buf));
		if (line == NULL)
			return (NULL);
		if (ret <= 0)
			return (line);
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret < 0)
		{
			free(line);
			return (NULL);
		}
		end_index = next_c_index(buf);
	}
	line = join_free(line, move_left(buf));
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	long long	ret;

	if (fd < 0)
		return (NULL);
	if (!(buf[0]))
	{
		ret = read(fd, &buf, BUFFER_SIZE);
		if (ret < 0 || (!ret && !(buf[0])))
			return (NULL);
		if (!ret)
			return (move_left(buf));
	}
	else
		ret = 1;
	return (join_until_nl(buf, fd, ret));
}
