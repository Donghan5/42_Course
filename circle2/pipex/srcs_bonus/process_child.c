/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:16:28 by donghank          #+#    #+#             */
/*   Updated: 2024/09/09 16:18:57 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// first assist to process infile
static int	process_assist_one(t_pipex *pipex, char **argv, int cmd_index)
{
	if (pipex->heredoc)
		pipex->infile = open(".heredoc", O_RDONLY);
	else
		pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
		return (handle_error(MSG_INFILE_ERROR), FAIL);
	close(pipex->tubes[cmd_index][0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	close(pipex->infile);
	if (dup2(pipex->tubes[cmd_index][1], STDOUT_FILENO) == -1)
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	close(pipex->tubes[cmd_index][1]);
	close(pipex->infile);
	return (SUCCESS);
}

// second assist to process outfile
static int	process_assist_two(t_pipex *pipex, char **argv, int cmd_index)
{
	pipex->outfile = open(argv[cmd_index + 1], \
		O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (pipex->outfile == -1)
		return (handle_error(MSG_INFILE_ERROR), FAIL);
	close(pipex->tubes[cmd_index][1]);
	if (dup2(pipex->tubes[cmd_index - 1][0], STDIN_FILENO) == -1)
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	close(pipex->tubes[cmd_index - 1][0]);
	close(pipex->outfile);
	return (SUCCESS);
}

static int	process_assist_three(t_pipex *pipex, int cmd_index)
{
	if (dup2(pipex->tubes[cmd_index - 1][0], STDIN_FILENO) == -1)
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	if (dup2(pipex->tubes[cmd_index][1], STDOUT_FILENO))
		return (handle_error(MSG_DUP2_ERROR), FAIL);
	close(pipex->tubes[cmd_index - 1][0]);
	close(pipex->tubes[cmd_index][1]);
	return (SUCCESS);
}

static void	close_tubes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->limit - 1)
	{
		close(pipex->tubes[i][0]);
		close(pipex->tubes[i][1]);
		i++;
	}
}

/*
**	get children process
**	infile = open infile
*/
int	child_process(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	if (cmd_index == pipex->start)
	{
		if (process_assist_one(pipex, argv, cmd_index) == FAIL)
			return (FAIL);
	}
	else if (cmd_index == pipex->limit - 1)
	{
		if (process_assist_two(pipex, argv, cmd_index) == FAIL)
			return (FAIL);
	}
	else
	{
		if (process_assist_three(pipex, cmd_index) == FAIL)
			return (FAIL);
	}
	close_tubes(pipex);
	ft_pipex(pipex, argv, envp, cmd_index);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
		handle_error_cleanup(pipex, MSG_EXEC_ERROR);
	return (SUCCESS);
}
