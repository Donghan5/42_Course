/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/06 00:58:42 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/*
**	put the element at the main heredoc func
**	ret = string which take the value of gnl func
*/
static void	here_doc_put(char **argv, int *p_fd)
{
	char	*ret;

	close(p_fd[0]);
	while (1)
	{
		ret = get_next_line(0);
		if (ft_strncmp(ret, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(ret);
			exit(0);
		}
		ft_putstr_fd(ret, p_fd[1]);
		free(ret);
	}
}

/*
**	do here_doc
**	p_fd = pipe fd
**	pid = process id
*/
static void	here_doc(char **argv)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		handle_error("Fail pipe");
	pid = fork();
	if (pid == -1)
		handle_error("Fail to generate pid");
	if (pid == 0)
		here_doc_put(argv, p_fd);
	else
	{
		close(p_fd[1]);
		if (dup2(p_fd[0], STDIN_FILENO) == -1)
		{
			close(p_fd[0]);
			handle_error("Fail to dup2");
		}
		close(p_fd[0]);
		wait(NULL);
	}
}

/* doing the process total pid 1 and pid 2 */
void doing_process(t_pipex *pipex, char **argv, char **envp)
{
	int i;

	i = 0;
	while (i < (pipex->here_doc ? 5 : 4))
	{
		if (pipe(pipex->tube) == -1)
			handle_error_cleanup(pipex, "Fail to pipe");
		pipex->pid1 = fork();
		if (pipex->pid1 == -1)
			handle_error_cleanup(pipex, "Fail to generate pid");
		if (pipex->pid1 == 0)
		{
			if (i == 2 && pipex->here_doc)
				here_doc(argv);
			else
				child_process(pipex, argv, envp);
		}
		pipex->pid2 = fork();
		if (pipex->pid2 == -1)
			handle_error_cleanup(pipex, "Fail to generate pid");
		if (pipex->pid2 == 0)
		{
			waitpid(pipex->pid1, NULL, 0);
			parent_process(pipex, argv, envp);
		}
		close(pipex->tube[0]);
		close(pipex->tube[1]);
		waitpid(pipex->pid1, NULL, 0);
		waitpid(pipex->pid2, NULL, 0);
		i++;
	}
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
	if (ft_strncmp(argv[1], "heredoc", 7) == 0)
	{
		if (argc != 6)
			handle_error("Make sure amount of the cmds");
		pipex.infile = open(argv[1], O_RDONLY);
		pipex.outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (pipex.infile == -1 || pipex.outfile == -1)
			handle_error_cleanup(&pipex, "Fail to open the files");
	}
	return (0);
}

/* 무한 루프 해결방법 valgrind 사용 시 */
