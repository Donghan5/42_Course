/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:00:40 by donghank          #+#    #+#             */
/*   Updated: 2024/09/08 18:00:50 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// initialize tubes and pids
static void	init_tubes_and_pids(t_pipex *pipex, int cmd_count)
{
	int	i;

	pipex->tubes = (int **)malloc(sizeof(int *) * (cmd_count - 1));
	i = 0;
	while (i < cmd_count - 1)
	{
		pipex->tubes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipex->tubes[i]) == -1)
			handle_error(MSG_PIPE);
	}
	pipex->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipex->pids)
		handle_error("Fail to allocate the memory");
}

/* initialize all element in the struct of the pipex */
void	init_pipex(t_pipex *pipex, int fd1, int fd2, int cmd_count)
{
	init_tubes_and_pids(pipex, cmd_count);
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
