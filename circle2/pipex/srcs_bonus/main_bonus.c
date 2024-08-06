/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/06 14:28:34 by donghank         ###   ########.fr       */
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
		ft_printf("heredoc>");
		buf = get_next_line(0);
		if (!buf)
			exit(EXIT_FAILURE);
		if (!ft_strncmp(delimiter, buf, ft_strlen(delimiter) + 1))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
		unlink(".heredoc_tmp");
}

/* helper func to static void doing_cmd_process */
static void	helper_cmd_process(t_pipex *pipex, char **argv, char **envp, int i)
{
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		handle_error_cleanup(pipex, "Fail to generate pid");
	if (pipex->pid1 == 0)
	{
		if (i == 2 && pipex->here_doc)
			child_process(pipex, argv, envp, 3);
		else
			child_process(pipex, argv, envp, i);
	}
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		handle_error_cleanup(pipex, "Fail to generate pid");
	if (pipex->pid2 == 0)
	{
		waitpid(pipex->pid1, NULL, 0);
		parent_process(pipex, argv, envp, i + 1);
	}
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
		helper_cmd_process(pipex, argv, envp, i);
		close(pipex->tube[0]);
		close(pipex->tube[1]);
		waitpid(pipex->pid1, NULL, 0);
		waitpid(pipex->pid2, NULL, 0);
		i++;
	}
}

/* doing the process total pid 1 and pid 2 */
void	doing_process(t_pipex *pipex, char **argv, char **envp)
{
	if (pipex->here_doc)
	{
		here_doc(argv[2], pipex);
		pipex->start = 3;
		pipex->limit = ft_strarray_len(argv) - 1;
	}
	else
	{
		pipex->start = 2;
		pipex->limit = ft_strarray_len(argv) - 1;
	}
	doing_cmd_process(pipex, argv, envp);
}

/*
**	problem leak in ls command,
**	still reachable is the character of the ls program. d
**	when I run the code with cat there aren't error
**	still reachable actually isn't memory leak.
*/
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
		handle_error("Make sure of your command line arguement");
	init_pipex(&pipex, -1, -1);
	pipex.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	if (pipex.here_doc && argc != 6)
		handle_error_cleanup(&pipex, "Make sure of amount of the cmds");
	doing_process(&pipex, argv, envp);
	cleanup(&pipex);
	return (0);
}
