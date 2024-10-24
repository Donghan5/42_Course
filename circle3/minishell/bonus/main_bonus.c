/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/25 01:23:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes_bonus/minishell_bonus.h"

/*
	Shell function for getting line from readline, works with Ctrl-D
*/
char	*get_line(t_env *env)
{
	char	*prompt;
	char	*line;

	prompt = get_prompt();
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		rl_clear_history();
		free_doub_array(env->environ);
		printf("exit\n");
		exit(env->sts);
	}
	if (*line)
		add_history(line);
	if (g_signal_received == SIGINT)
		env->sts = 130;
	if (g_signal_received == SIGQUIT)
		env->sts = 131;
	if (g_signal_received)
		g_signal_received = 0;
	return (line);
}

void	parse_and_run(char **line, t_env *env)
{
	char		***tokens;
	t_glob_pipe	*glob_pipe;

	tokens = pre_parsing(*line, env);
	free(*line);
	if (!tokens)
		return ;
	if (is_lex_error(tokens) || !parse(&glob_pipe, tokens))
	{
		free_triple_tokens(tokens);
		env->sts = 2;
		return ;
	}
	free_triple_tokens(tokens);
	if (prepare_pipeline(glob_pipe, env))
		run_global_pipeline(&glob_pipe, env);
	else if (!env->sts)
		env->sts = 1;
	free_glob_pipe(&glob_pipe);
}

void	parse_env(t_env *env, char **envs)
{
	int	i;

	i = 0;
	env->sts = 0;
	env->environ = NULL;
	env->environ_name_value = NULL;
	env->is_interactive = 1;
	while (envs && envs[i])
		i++;
	env->environ = malloc((i + 1) * sizeof(char *));
	if (!env->environ)
		exit_error("malloc");
	i = 0;
	while (envs && envs[i])
	{
		env->environ[i] = ft_strdup(envs[i]);
		if (!env->environ)
			return (free_doub_array(env->environ), exit_error("malloc"));
		i++;
	}
	env->environ[i] = NULL;
}

/*
	Non-interactive mode execution
*/
void	read_from_files(t_env *env, int argc, char **argv)
{
	char	*line;
	int		fd;

	fd = 0;
	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (print_file_err(argv[1]),
				free_doub_array(env->environ), exit(127));
	}
	line = get_next_line(fd);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (*line)
			parse_and_run(&line, env);
		else
			free(line);
		line = get_next_line(fd);
	}
	if (fd != 0)
		close (fd);
	return (free_doub_array(env->environ), exit(env->sts));
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	env;

	parse_env(&env, envp);
	increment_shell_level(&env);
	header();
	using_history();
	set_signal();
	if (!isatty(STDIN_FILENO) || argc > 1)
	{
		env.is_interactive = 0;
		read_from_files(&env, argc, argv);
	}
	while (1)
	{
		line = get_line(&env);
		if (!*line)
		{
			free(line);
			continue ;
		}
		parse_and_run(&line, &env);
	}
	rl_clear_history();
	return (free_doub_array(env.environ), 1);
}
