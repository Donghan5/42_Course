/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/11 22:52:03 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this to handle eof(user input ctrl D)
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
	else
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

void	read_from_stdin(t_env *env)
{
	char	*line;

	line = get_next_line(0);
	env->sts = 0;
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (*line)
			parse_and_run(&line, env);
		line = get_next_line(0);
	}
	free_doub_array(env->environ);
	exit(env->sts);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	env;

	(void)argc;
	(void)argv;
	parse_env(&env, envp);
	increment_shell_level(&env);
	// header();
	using_history();
	set_signal();
	// if (!isatty(STDIN_FILENO))
	// 	read_from_stdin(&env);
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
	free_doub_array(env.environ);
	return (0);
}
