/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:04:13 by donghank          #+#    #+#             */
/*   Updated: 2024/10/13 22:01:43 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// this part to SIGINT part
static void	help_sigint(pid_t pid)
{
	if (pid == -1)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("^C\n", STDOUT);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		ft_putstr_fd("\n", STDOUT);
}

// SIGINT is the ctrl c and SIGQUIT is ctrl '\'
void	handle_signal(int signo)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signo == SIGINT)
		help_sigint(pid);
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

// to alert the signal nums by using the signal macor
void	set_signal(void)
{
	t_sigaction	sa_quit;
	t_sigaction	sa_int;
	t_termios	term;

	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_int.sa_handler = handle_signal;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("sigaction");
		return ;
	}
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("sigaction");
		return ;
	}
	memset(&term, 0, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// to help signal of the heredoc
void	sigint_heredoc(int sig)
{
	t_env	*env;

	(void)sig;
	write(1, "\n", 1);
	env->sts = 130;
}
