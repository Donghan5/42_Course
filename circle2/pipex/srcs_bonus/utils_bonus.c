/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:42:29 by donghank          #+#    #+#             */
/*   Updated: 2024/09/08 17:48:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_cmd_args(char **cmd_args)
{
	int	i;

	if (!cmd_args)
		return ;
	i = 0;
	while (cmd_args[i] != NULL)
	{
		free(cmd_args[i]);
		i++;
	}
	free(cmd_args);
}

/* free all of the memories paths */
void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i] != NULL)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

/* to handle the errors */
void	handle_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	cleanup(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (pipex->path)
		free(pipex->path);
	if (pipex->cmd_args)
		free_cmd_args(pipex->cmd_args);
	if (pipex->paths)
		free_paths(pipex->paths);
	if (pipex->infile != -1)
		close(pipex->infile);
	if (pipex->outfile != -1)
		close(pipex->outfile);
	if (pipex->tubes)
	{
		while (i++ < pipex->limit - 1)
			close(*pipex->tubes[i]);
	}
	i = 0;
	if (pipex->pids)
	{
		while (i < pipex->limit - 1)
			close(pipex->pids[i++]);
	}
	free(pipex->pids);
}

/*
**	to calcuate the length of the array
**	return val = the length of the array
*/
int	ft_strarray_len(char **arr)
{
	int	len;

	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}
