/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/21 16:30:16 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* doing the process total pid 1 and pid 2 */
void	doing_process(t_pipex *pipex, char **argv, char **envp)
{
	if (pipe(pipex->tube) == -1)
		handle_error_cleanup(pipex, "Fail to create pipe");
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		handle_error_cleanup(pipex, "Fail fork");
	if (pipex->pid1 == 0)
		child_process(pipex, argv, envp);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		handle_error_cleanup(pipex, "Fail fork");
	if (pipex->pid2 == 0)
		parent_process(pipex, argv, envp);
	close(pipex->tube[0]);
	close(pipex->tube[1]);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, NULL, 0);
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
		handle_error("Make sure of your command line arguement");
	if (argc == 5)
	{
		init_pipex(&pipex, -1, -1);
		doing_process(&pipex, argv, envp);
		cleanup(&pipex);
	}
	return (0);
}

/* 무한 루프 해결방법 valgrind 사용 시 */
