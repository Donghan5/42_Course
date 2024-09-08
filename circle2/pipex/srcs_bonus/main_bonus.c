/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:01:44 by donghank          #+#    #+#             */
/*   Updated: 2024/09/08 18:01:02 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
		handle_error(USAGE);
	cmd_count = argc - 3;
	init_pipex(&pipex, -1, -1, cmd_count);
	pipex.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	if (pipex.here_doc && argc != 6)
		handle_error_cleanup(&pipex, USAGE);
	if (!pipex.here_doc && argc < 5)
		handle_error_cleanup(&pipex, USAGE);
	doing_process(&pipex, argv, envp, cmd_count);
	cleanup(&pipex);
	if (pipex.here_doc)
		unlink(".heredoc_tmp");
	return (0);
}
