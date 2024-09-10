/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:04:13 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 22:26:18 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// assingnement
// ctrl-C in an empty prompt should display a new line with a new prompt.
// ctrl-\ in an empty prompt should not do anything.
// ctrl-D in an empty prompt should quit minishell --> RELAUNCH!
// ctrl-C in a prompt after you wrote some stuff should display a new line with a new prompt.
// The buffer should be clean too. Press "Enter" to make sure nothing from the previous line is executed.
// ctrl-D in a prompt after you wrote some stuff should not do anything.
// ctrl-\ in a prompt after you wrote some stuff should not do anything.
// Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
// Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.
// Try ctrl-D after running a blocking command like cat without arguments or grep “something“.

static void	help_sigint(pid_t pid)
{
	if (pid == -1)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd(" \n", STDOUT);
		rl_on_newline();
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
	if (signo == SIGQUIT)
	{
		if (pid == -1)
		{
			rl_on_newline();
			rl_redisplay();
			ft_putstr_fd("  \b\b", STDOUT);
		}
		else
			ft_putstr_fd("Quit: 3\n", STDOUT);
	}
}

void	set_signal(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
}
