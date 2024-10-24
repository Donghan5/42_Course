/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:04:13 by donghank          #+#    #+#             */
/*   Updated: 2024/10/24 15:02:18 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// this part to SIGINT part
static void	help_sigint(pid_t pid)
{
	if (pid == -1)
	{
		rl_redisplay();
		rl_on_new_line();
		ft_putstr_fd("^C\n", STDOUT);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		ft_putstr_fd("\n", STDOUT);
	g_signal_received = 130;
}

// SIGINT is the ctrl c and SIGQUIT is ctrl '\'
void	handle_signal(int signo)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signo == SIGINT)
	{
		help_sigint(pid);
		g_signal_received = 130;
	}
	if (signo == SIGQUIT)
	{
		if (pid == -1)
		{
			rl_on_new_line();
			rl_redisplay();
			ft_putstr_fd("  \b\b", STDOUT);
		}
		else
			ft_putstr_fd("Quit (core dumped)\n", STDOUT);
		g_signal_received = 131;
	}
}

// to alert the signal nums by using the signal macor
void	set_signal(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
}
