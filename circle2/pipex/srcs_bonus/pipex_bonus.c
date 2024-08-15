/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:45:04 by donghank          #+#    #+#             */
/*   Updated: 2024/08/15 12:45:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/* initialize all element in the struct of the pipex */
void	init_pipex(t_pipex *pipex, int fd1, int fd2)
{
	pipex->pid1 = -1;
	pipex->tube[0] = -1;
	pipex->tube[1] = -1;
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
	if (pipex->here_doc && cmd_index == pipex->start)
		pipex->infile = open(".heredoc_tmp", O_RDONLY);
	else
		pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
		handle_error_cleanup(pipex, "Fail to open infile");
	close(pipex->tube[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail to dup2 stdin (child)");
	if (dup2(pipex->tube[1], STDOUT_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail to dup2 stdout (child)");
	close(pipex->tube[1]);
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
	if (cmd_index == pipex->limit - 1)
	{
		pipex->outfile = open(argv[cmd_index], \
			O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (pipex->outfile == -1)
			handle_error_cleanup(pipex, "Fail to open outfile");
	}
	else if (cmd_index < pipex->limit - 1)
		pipex->outfile = pipex->tube[1];
	close(pipex->tube[1]);
	if (dup2(pipex->tube[0], STDIN_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail dup2 stdin (parent)");
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		handle_error_cleanup(pipex, "Fail dup2 stdout (parent)");
	close(pipex->tube[0]);
	ft_pipex(pipex, argv, envp, cmd_index);
	if (cmd_index == pipex->limit - 1)
		close(pipex->outfile);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
		handle_error_cleanup(pipex, "Fail execve (parent)");
}

/* helper func to economize the lines */
void	handle_error_cleanup(t_pipex *pipex, char *msg)
{
	handle_error(msg);
	cleanup(pipex);
}
