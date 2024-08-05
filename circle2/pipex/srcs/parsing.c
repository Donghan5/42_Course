/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:42:35 by donghank          #+#    #+#             */
/*   Updated: 2024/08/04 22:05:37 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* free helper path to economize the lines */
static void	free_helper1(t_pipex *pipex)
{
	free(pipex->path);
	pipex->path = NULL;
}

/* free helper paths to economize the lines */
static void	free_helper2(t_pipex *pipex)
{
	free_paths(pipex->paths);
	pipex->paths = NULL;
}

/*
**	join the path by separating char /
**	full_path = all of the path of the cmd
**	tmp_path = temp path
*/
static char	*join_path(const char *path, const char *cmd)
{
	char	*full_path;
	char	*tmp_path;
	size_t	path_len;

	path_len = ft_strlen(path);
	tmp_path = NULL;

	if (path[path_len - 1] == '/')
		full_path = ft_strjoin(path, cmd);
	else
	{
		tmp_path = ft_strjoin(path, "/");
		if (!tmp_path)
			return (handle_error("Fail to make tmp_path"), NULL);
		full_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
	}
	if (!full_path)
		return (handle_error("Fail to alloc"), NULL);
	return (full_path);
}

/*
**	to search the paths in environment path
*/
char	**get_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split((*envp) + 5, ':'));
}

/* to search the command line path */
char	*find_path(t_pipex *pipex, char **envp)
{
	int	i;

	if (pipex->cmd[0] == '/' || (pipex->cmd[0] == '.' && pipex->cmd[1] == '/'))
	{
		if (access(pipex->cmd, X_OK) == 0)
			return (pipex->cmd);
		return (NULL);
	}
	pipex->paths = get_paths(envp);
	if (!pipex->paths)
		return (NULL);
	i = 0;
	while (pipex->paths[i])
	{
		pipex->path = join_path(pipex->paths[i], pipex->cmd);
		if (access(pipex->path, X_OK) == 0)
			return (pipex->path);
		free_helper1(pipex);
		i++;
	}
	free_helper2(pipex);
	return (NULL);
}
