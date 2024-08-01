/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:42:21 by donghank          #+#    #+#             */
/*   Updated: 2024/07/28 16:49:37 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* initialize all element in the struct of the pipex */
void	init_pipex(t_pipex *pipex, int fd1, int fd2)
{
	pipex->pid1 = -1;
	pipex->tube[0] = -1;
	pipex->tube[1] = -1;
	pipex->infile = fd1;
	pipex->outfile = fd2;
	pipex->path = NULL;
	pipex->paths = NULL;
	pipex->cmd = NULL;
	pipex->cmd_args = NULL;
}

/*
**	get children process
**	infile = open infile
*/
void	child_process(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
	{
		handle_error("Fail to open infile\n");
		cleanup(pipex);
	}
	close(pipex->tube[0]);
	if (dup2(pipex->tube[1], STDOUT_FILENO) == -1)
	{
		cleanup(pipex);
		handle_error("Fail dup\n");
	}
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
	{
		cleanup(pipex);
		handle_error("Fail dup\n");
	}
	cleanup(pipex);
	ft_pipex(pipex, argv, envp, 2);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
	{
		cleanup(pipex);
		handle_error("Fail\n");
	}
}

/*
**	get parent process
**	outfile = outfile the last thing in the command
*/
void	parent_process(t_pipex *pipex, char **argv, char **envp)
{
	pipex->outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex->outfile == -1)
	{
		handle_error("Fail to open outfile");
		cleanup(pipex);
	}
	close(pipex->tube[1]);
	if (dup2(pipex->tube[0], STDIN_FILENO) == -1)
	{
		cleanup(pipex);
		handle_error("Fail dup\n");
	}
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
	{
		cleanup(pipex);
		handle_error("Fail dup\n");
	}
	cleanup(pipex);
	ft_pipex(pipex, argv, envp, 3);
	if (execve(pipex->path, pipex->cmd_args, envp) == -1)
	{
		cleanup(pipex);
		handle_error("Fail\n");
	}
}
