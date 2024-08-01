/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:42:35 by donghank          #+#    #+#             */
/*   Updated: 2024/07/28 16:37:28 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* to search the command line path */
char	*find_path(t_pipex *pipex, char **envp)
{
	int		i;
	char	*tmp_path;

	while (*envp && ft_strncmp("PATH", *envp, 4) != 0)
		envp++;
	pipex->paths = ft_split((*envp) + 5, ':');
	if (!pipex->paths)
		return (NULL);
	i = 0;
	while (pipex->paths[i])
	{
		tmp_path = ft_strjoin(pipex->paths[i], "/");
		pipex->path = ft_strjoin(tmp_path, pipex->cmd);
		free(tmp_path);
		if (access(pipex->path, X_OK) == 0)
			return (free_paths(pipex->paths), pipex->path);
		free(pipex->path);
		pipex->path = NULL;
		i++;
	}
	free_paths(pipex->paths);
	pipex->paths = NULL;
	return (NULL);
}

/*
**	implement pipe-line
**	cmd_index = to verify the command arguemet like "wc -l", index
*/
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	pipex->cmd_args = ft_split(argv[cmd_index], ' ');
	if (pipex->cmd_args == NULL)
	{
		cleanup(pipex);
		handle_error("Fail the read command\n");
	}
	pipex->cmd = pipex->cmd_args[0];
	pipex->path = find_path(pipex, envp);
	if (pipex->path == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
		cleanup(pipex);
		exit(127);
	}
}
