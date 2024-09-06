/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 21:25:46 by donghank          #+#    #+#             */
/*   Updated: 2024/09/06 22:52:30 by donghank         ###   ########.fr       */
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
		if (!ft_strncmp(delimiter, buf, ft_strlen(delimiter)) \
			&& buf[ft_strlen(delimiter)] == '\n')
			break ;
		ft_putstr_fd(buf, file);
		ft_putchar_fd('\n', file);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
		unlink(".heredoc_tmp");
}

/* doing pipe opreation */
void	doing_process(t_pipex *pipex, char **argv, char **envp, int cmd_count)
{
	int	i;

	i = 0;
	if (pipe(pipex->tube) == -1)
		handle_error_cleanup(pipex, "Fail to create pipe");
	while (i < cmd_count)
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] == 0)
		{
			if (pipex->here_doc)
			{
				here_doc(argv[2], pipex);
				child_process(pipex, argv, envp, i + 1);
			}
			child_process(pipex, argv, envp, i);
		}
		parent_process(pipex, argv, envp, i + 1);
		i++;
		waitpid(pipex->pids[i + 1], NULL, 0);
	}
	close(pipex->tube[0]);
	close(pipex->tube[1]);
}
