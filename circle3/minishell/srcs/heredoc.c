/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:33:12 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/15 17:56:51 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	g_signal_received;

static void	heredoc_sigint_handler(int signo)
{
	ft_putstr_fd("^C", STDOUT);
	g_signal_received = signo;
	rl_done = 1;
}

static int	event_hook(void)
{
	if (g_signal_received)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

int	ft_heredoc(char *stop_word, int fd)
{
	char	*line;

	while (1)
	{
		g_signal_received = 0;
		line = readline("> ");
		if (!line || g_signal_received)
			break ;
		if (!ft_strncmp(stop_word, line, ft_strlen(stop_word) + 1))
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	if (line)
		free(line);
	if (!line && !g_signal_received)
		printf("%s (wanted `%s')\n", HDOC_ERR, stop_word);
	rl_event_hook = NULL;
	rl_done = 0;
	if (g_signal_received == SIGINT)
		return (0);
	return (1);
}

int	setup_heredoc(t_glob_pipe *current, t_glob_pipe *next, int *fd, t_env *env)
{
	int			heredoc_status;
	t_sigaction	sa;

	*fd = open("/tmp/sh-thd-86500896238475834",
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
		return (print_file_err("/tmp/sh-thd-86500896238475834"));
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	rl_event_hook = event_hook;
	heredoc_status = ft_heredoc(next->name, *fd);
	smart_close(*fd);
	set_signal();
	g_signal_received = 0;
	if (!heredoc_status)
		return (unlink("/tmp/sh-thd-86500896238475834"), env->sts = 130, 0);
	*fd = open("/tmp/sh-thd-86500896238475834", O_RDONLY);
	if ((*fd) < 0)
		return (unlink("/tmp/sh-thd-86500896238475834"), env->sts = 1,
			print_file_err("/tmp/sh-thd-86500896238475834"));
	current->redir_io[0] = *fd;
	unlink("/tmp/sh-thd-86500896238475834");
	return (1);
}
