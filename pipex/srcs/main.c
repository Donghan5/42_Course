/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/07/30 14:58:06 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
**	problem leak in ls command,
**	still reachable is the character of the ls program.
**	when I run the code with cat there aren't error
**	still reachable actually isn't memory leak.
*/
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		handle_error("ERROR cmd line\n");
	if (argc == 5)
	{
		init_pipex(&pipex, -1, -1);
		if (pipe(pipex.tube) == -1)
		{
			handle_error("Fail pipe\n");
			cleanup(&pipex);
		}
		pipex.pid1 = fork();
		if (pipex.pid1 == -1)
		{
			handle_error("Fail to fork.");
			cleanup(&pipex);
		}
		if (pipex.pid1 == 0)
			child_process(&pipex, argv, envp);
		waitpid(pipex.pid1, NULL, WNOHANG);
		parent_process(&pipex, argv, envp);
		cleanup(&pipex);
	}
	return (0);
}
