/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 21:25:46 by donghank          #+#    #+#             */
/*   Updated: 2024/08/16 11:50:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/*
**	doing here_doc func
**	file = open temp here_doc file
**	buf = to read in the standard input(gnl)
*/
static void	here_doc(char *delimiter, t_pipex *pipex)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file == -1)
		handle_error("Fail to open the file");
	while (1)
	{
		ft_printf("[heredoc] ");
		buf = get_next_line(0);
		if (!buf)
			exit(EXIT_FAILURE);
		if (!ft_strncmp(delimiter, buf, ft_strlen(delimiter)))
			break ;
		ft_putstr_fd(buf, file);
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
		unlink(".heredoc_tmp");
}

/* handle child process */
static void	handle_child(t_pipex *pipex, char **argv, char **envp, int i)
{
	if (i == pipex->start && pipex->here_doc)
		child_process(pipex, argv, envp, i + 1);
	else
		child_process(pipex, argv, envp, i);
}

/* handle parent process */
static void	handle_parent_close(t_pipex *pipex)
{
	close(pipex->tube[0]);
	close(pipex->tube[1]);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, NULL, 0);
}

/*
**	separate func doing_process due to limit the line
**	i = start;
*/
static void	doing_cmd_process(t_pipex *pipex, char **argv, char **envp)
{
	int	i;

	i = pipex->start;
	while (i < pipex->limit)
	{
		if (pipe(pipex->tube) == -1)
			handle_error_cleanup(pipex, "Fail to pipe");
		pipex->pid1 = fork();
		if (pipex->pid1 == -1)
			handle_error_cleanup(pipex, "Fail to generate pid");
		if (pipex->pid1 == 0)
			handle_child(pipex, argv, envp, i);
		else
		{
			pipex->pid2 = fork();
			if (pipex->pid2 == -1)
				handle_error_cleanup(pipex, "Fail to generate pid");
			if (pipex->pid2 == 0)
				parent_process(pipex, argv, envp, i + 1);
			else
				handle_parent_close(pipex);
		}
		i++;
	}
}

/* doing the process total pid 1 and pid 2 */
void	doing_process(t_pipex *pipex, int argc, char **argv, char **envp)
{
	if (pipex->here_doc)
	{
		here_doc(argv[2], pipex);
		pipex->start = 3;
		pipex->limit = argc - 2;
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile == -1)
			handle_error_cleanup(pipex, "Fail to open infile");
		pipex->start = 2;
		pipex->limit = argc - 1;
	}
	pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		handle_error_cleanup(pipex, "Fail to open outfile");
	if (pipe(pipex->tube) == -1)
		handle_error_cleanup(pipex, "Fail to pipe");
	doing_cmd_process(pipex, argv, envp);
}
