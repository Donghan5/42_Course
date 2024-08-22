/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/08/22 12:15:58 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/* Closing all the pipes */
void	close_all_pipe(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (pipex->tube)
	{
		while (i < 2 * pipex->tube_count)
		{
			if (pipex->tube[i] != -1)
				close(pipex->tube[i]);
			i++;
		}
	}
}

/* helper func to economize the lines */
void	handle_error_cleanup(t_pipex *pipex, char *msg)
{
	handle_error(msg);
	cleanup(pipex);
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
	int		cmd_count;

	if (argc < 5)
		handle_error("Make sure of your command line arguement");
	cmd_count = argc - 3;
	init_pipex(&pipex, -1, -1, cmd_count);
	pipex.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	if (pipex.here_doc && argc != 6)
		handle_error_cleanup(&pipex, \
		"Usage: ./piepx here_doc LIMITER cmd1 cmd2 outfile");
	if (!pipex.here_doc && argc < 5)
		handle_error_cleanup(&pipex, \
		"Usage: ./pipex infile cmd1 cmd2 ... outfile");
	doing_process(&pipex, argc, argv, envp);
	cleanup(&pipex);
	if (pipex.here_doc)
		unlink(".heredoc_tmp");
	return (0);
}
