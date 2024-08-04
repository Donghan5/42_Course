/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/04 16:44:27 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* doing the process total pid 1 and pid 2 */
void	doing_process(t_pipex *pipex, char **argv, char **envp)
{
	if (pipe(pipex->tube) == -1)
	{
		handle_error("Fail pipe");
		cleanup(pipex);
	}
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
	{
		handle_error("Fail to fork");
		cleanup(pipex);
	}
	if (pipex->pid1 == 0)
		child_process(pipex, argv, envp);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
	{
		handle_error("Fail fork");
		cleanup(pipex);
	}
	if (pipex->pid2 == 0)
	{
		waitpid(pipex->pid1, NULL, 0);
		parent_process(pipex, argv, envp);
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

	if (argc != 5)
		handle_error("ERROR cmd line");
	if (argc == 5)
	{
		init_pipex(&pipex, -1, -1);
		doing_process(&pipex, argv, envp);
		cleanup(&pipex);
	}
	return (0);
}

/* 무한 루프 해결방법 valgrind 사용 시 */
