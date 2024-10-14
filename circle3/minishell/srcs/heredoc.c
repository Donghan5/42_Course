/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:33:12 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/14 17:32:48 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// using gloval variable to track the status of the heredoc
volatile int	g_signal_received;

/*
	handler SIGINT in case of the heredoc
*/
static void	heredoc_sigint_handler(int signo)
{
	ft_putstr_fd("^C", STDOUT);
	g_signal_received = signo;
	rl_done = 1;
}

/*
	make the event when SIGINT
*/
static int	event_hook(void)
{
	if (g_signal_received)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

/*
	heredoc func, when ctrl c input, exit the mode with the ctrl c
	when ctrl d, exit like bash (exit process and run the command)
	@param stop_word = limiter of the heredoc
*/
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
	if (g_signal_received == SIGINT)
		return (0);
	return (1);
}

/*
	prepare all the action of heredoc
*/
int	setup_heredoc(t_glob_pipe *current, t_glob_pipe *next, int *fd, t_env *env)
{
	int			heredoc_status;
	t_sigaction	sa;
	t_sigaction	old_sa;

	*fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
		return (print_file_err(".heredoc"));
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	rl_event_hook = event_hook;
	heredoc_status = ft_heredoc(next->name, *fd);
	sigaction(SIGINT, &old_sa, NULL);
	close(*fd);
	g_signal_received = 0;
	if (!heredoc_status)
		return (env->sts = 130, 0);
	*fd = open(".heredoc", O_RDONLY);
	if (*fd < 0)
		return (env->sts = 1, print_file_err(".heredoc"));
	unlink(".heredoc");
	current->redir_io[0] = *fd;
	return (1);
}
