/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/22 15:42:40 by donghank         ###   ########.fr       */
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

char	mark_literal(char	*str)
{
	int	i;

	i = 0;
	if (!str || !*str || !is_quote(*str))
		return (0);
	while (str[i] != '\0')
		i++;
	if ((i > 1) && (is_quote(str[i - 1])))
		return (1);
	return (0);
}

void	parse_and_run(char **line, t_env *env, int *status)
{
	char		***tokens;
	t_glob_pipe	*glob_pipe;

	tokens = pre_parsing(*line, env);
	free(*line);
	if (!tokens)
		return ;
	if (!parse(&glob_pipe, tokens))
		return ;
	if (prepare_pipeline(glob_pipe))
		run_global_pipeline(glob_pipe, env, status);
	else
		*status = 1;
	free_glob_pipe(&glob_pipe);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_env		env;
	extern char	**environ;
	int			status;

	status = 0;
	init_env(&env);
	parse_env(&env, environ);
	increment_shell_level(&env);
	header();
	using_history();
	set_signal();
	while (1)
	{
		line = get_line(status);
		if (!*line)
		{
			free(line);
			continue ;
		}
		parse_and_run(&line, &env, &status);
	}
	rl_clear_history();
	return (0);
}
