/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:42:35 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 23:29:14 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_helper(t_pipex *pipex)
{
	free(pipex->path);
	pipex->path = NULL;
}

/* to search the command line path */
char	*find_path(t_pipex *pipex, char **envp)
{
	int		i;
	char	*tmp_path;

	i = 0;
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	pipex->paths = ft_split((envp[i]) + 5, ':');
	if (!pipex->paths)
		return (NULL);
	i = 0;
	while (pipex->paths[i])
	{
		tmp_path = ft_strjoin(pipex->paths[i], "/");
		pipex->path = ft_strjoin(tmp_path, pipex->cmd);
		free(tmp_path);
		ft_printf("Testing path: %s\n", pipex->path);
		if (access(pipex->path, X_OK) == 0)
			return (pipex->path);
		free_helper(pipex);
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
		handle_error("Fail the read command");
	}
	pipex->cmd = pipex->cmd_args[0];
	pipex->path = find_path(pipex, envp);
	if (pipex->path == NULL)
	{
		handle_error("Command not found");
		cleanup(pipex);
		exit(127);
	}
}
