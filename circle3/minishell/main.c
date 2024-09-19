/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/20 00:49:05 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history,
// printf, malloc, free, write, access, open, read,
// close, fork, wait, waitpid, wait3, wait4, signal,
// sigaction, sigemptyset, sigaddset, kill, exit,
// getcwd, chdir, stat, lstat, fstat, unlink, execve,
// dup, dup2, pipe, opendir, readdir, closedir,
// strerror, perror, isatty, ttyname, ttyslot, ioctl,
// getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
// tgetnum, tgetstr, tgoto, tputs

// this to handle eof(user input ctrl D)
char	*get_line(int status)
{
	char	*prompt;
	char	*line;

	prompt = get_prompt();
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		rl_clear_history();
		printf("exit\n");
		exit(status);
	}
	if (*line)
		add_history(line);
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_env		env;
	extern char	**environ;
	t_glob_pipe	*glob_pipe;
	int			status;

	status = 0;
	parse_env(&env, environ);
	header();
	using_history();
	set_signal();
	while (1)
	{
		line = get_line(status);
		char *expanded_line = expander(line, env.environ);
		printf("%s\n", expanded_line);
		// if (!*line || !parse(&glob_pipe, line))
		// {
		// 	free(line);
		// 	continue ;
		// }
		// free(line);
		// if (prepare_pipeline(glob_pipe))
		// 	run_global_pipeline(glob_pipe, &env, &status);
		// free_glob_pipe(&glob_pipe);
	}
	rl_clear_history();
	return (0);
}
