/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:33:12 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/25 13:16:45 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

volatile int	g_signal_received;

static void	heredoc_sigint_handler(int signo)
{
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

int	ft_heredoc(char *stop_word, int fd, t_env *env)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		g_signal_received = 0;
		line = readline("> ");
		if (!line || (g_signal_received == SIGINT))
			break ;
		if (!ft_strncmp(stop_word, line, ft_strlen(stop_word) + 1))
			break ;
		expanded_line = expander_heredoc(line, env);
		if (!expanded_line)
			return (free(line), rl_event_hook = NULL, rl_done = 0, 0);
		ft_putendl_fd(expanded_line, fd);
		free(line);
		free(expanded_line);
	}
	if (line)
		free(line);
	if (!line && (g_signal_received != SIGINT))
		printf("%s (wanted `%s')\n", HDOC_ERR, stop_word);
	if (g_signal_received == SIGINT)
		return (rl_event_hook = NULL, rl_done = 1, 0);
	return (rl_event_hook = NULL, rl_done = 1, 1);
}

int	setup_heredoc(t_glob_pipe *current, t_glob_pipe *next, int *fd, t_env *env)
{
	int	heredoc_status;

	*fd = open(HDOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
		return (print_file_err(HDOC_FILE));
	signal(SIGINT, heredoc_sigint_handler);
	rl_event_hook = event_hook;
	heredoc_status = ft_heredoc(next->name, *fd, env);
	smart_close(*fd);
	set_signal();
	if (g_signal_received == SIGINT)
		return (unlink(HDOC_FILE), 0);
	*fd = open(HDOC_FILE, O_RDONLY);
	if ((*fd) < 0)
		return (unlink(HDOC_FILE), env->sts = 1, print_file_err(HDOC_FILE));
	current->redir_io[0] = *fd;
	unlink(HDOC_FILE);
	return (1);
}
