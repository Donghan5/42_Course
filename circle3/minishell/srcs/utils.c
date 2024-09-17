/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:20:36 by donghank          #+#    #+#             */
/*   Updated: 2024/09/17 12:02:47 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arr(char **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}

void	free_doub_array(void **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free (arr[i]);
		i++;
	}
	if (arr)
		free (arr);
}

char	*triple_strjoin(char *s1, char *s2, char *s3)
{
	char	*new_str;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	new_str = ft_strjoin(tmp, s3);
	free (tmp);
	if (!new_str)
		return (NULL);
	return (new_str);
}

// to get the hostname == maybe put the gnl in files..?
char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	char		line[70000];
	static int	buffer_readed;
	static int	buffer_pos;
	int			i;

	i = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (buffer_pos >= buffer_readed)
		{
			buffer_readed = read(fd, buffer, BUFFER_SIZE);
			buffer_pos = 0;
			if (buffer_readed <= 0)
				break ;
		}
		line[i++] = buffer[buffer_pos++];
		if (line[i - 1] == '\n')
			break ;
	}
	line[i] = '\0';
	if (i == 0)
		return (NULL);
	return (ft_strdup(line));
}
