/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:45:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/08 17:58:30 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
**	get parent process
**	outfile = outfile the last thing in the command
*/
void	parent_process(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->limit - 1)
	{
		close(pipex->tubes[i][0]);
		close(pipex->tubes[i][1]);
		i++;
	}
	i = 0;
	while (i < pipex->limit)
	{
		waitpid(*pipex->tubes[i], NULL, 0);
		i++;
	}
}
