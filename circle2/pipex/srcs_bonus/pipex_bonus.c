/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:45:04 by donghank          #+#    #+#             */
/*   Updated: 2024/08/22 15:16:01 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	init_pid_tube(t_pipex *pipex, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	pipex->tube_count = cmd_count - 1;
	pipex->tube = malloc(sizeof(int) * 2 * pipex->tube_count);
	if (!pipex->tube)
		handle_error("Fail to allocate mem for tubes");
	while (i < 2 * pipex->tube_count)
	{
		pipex->tube[i] = -1;
		i++;
	}
	pipex->pid_count = cmd_count - 1;
	pipex->pid = malloc(sizeof(pid_t) * pipex->pid_count);
	if (!pipex->pid)
		handle_error("Fail to allocate mem for pids");
	while (j < pipex->pid_count)
	{
		pipex->pid[j] = -1;
		j++;
	}
}

/* initialize all element in the struct of the pipex */
void	init_pipex(t_pipex *pipex, int fd1, int fd2, int cmd_count)
{
	init_pid_tube(pipex, cmd_count);
	pipex->here_doc = 0;
	pipex->start = 0;
	pipex->limit = 0;
	pipex->infile = fd1;
	pipex->outfile = fd2;
	pipex->path = NULL;
	pipex->paths = NULL;
	pipex->cmd = NULL;
	pipex->cmd_args = NULL;
}

/*
**	implement pipe-line
**	cmd_index = to verify the command arguemet like "wc -l", index
*/
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	pipex->cmd_args = ft_split(argv[cmd_index], ' ');
	if (pipex->cmd_args == NULL)
		handle_error_cleanup(pipex, "Fail to read the commands");
	pipex->cmd = pipex->cmd_args[0];
	pipex->path = find_path(pipex, envp);
	if (pipex->path == NULL)
		handle_error_cleanup(pipex, "Command not found");
}

/*
**	get children process
**	infile = open infile
*/
void	child_process(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	if (cmd_index > pipex->start)
		close(pipex->tube[2 * (cmd_index - pipex->start) - 2]);
	if (cmd_index < pipex->limit - 1)
		close(pipex->tube[2 * (cmd_index - pipex->start) + 1]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail to dup2 stdin (child)");
	if (dup2(pipex->tube[2 * (cmd_index - pipex->start) + 1], \
	STDOUT_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail to dup2 stdout (child)");
	close(pipex->tube[2 * (cmd_index - pipex->start) + 1]);
	close(pipex->infile);
	ft_pipex(pipex, argv, envp, cmd_index);
	cleanup(pipex);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
		handle_error_cleanup(pipex, "Fail execve (child)");
}

/*
**	get parent process
**	outfile = outfile the last thing in the command
*/
void	parent_process(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	if (cmd_index < pipex->limit - 1)
		pipex->outfile = pipex->tube[2 * (cmd_index - pipex->start) + 1];
	close(pipex->tube[2 * (cmd_index - pipex->start) + 1]);
	if (dup2(pipex->tube[2 * (cmd_index - pipex->start)], STDIN_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail dup2 stdin (parent)");
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail dup2 stdout (parent)");
	close(pipex->tube[2 * (cmd_index - pipex->start)]);
	ft_pipex(pipex, argv, envp, cmd_index);
	if (cmd_index == pipex->limit - 1)
		close(pipex->outfile);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
		handle_error_cleanup(pipex, "Fail execve (parent)");
}
