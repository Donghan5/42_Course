/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/24 17:27:40 by donghank         ###   ########.fr       */
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
		free_doub_array((void **)env->environ);
		printf("exit\n");
		exit(*env->status);
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
	if (!parse(&glob_pipe, tokens))
	{
		free_triple_tokens(tokens);
		*env->status = 1;
		return ;
	}
	free_triple_tokens(tokens);
	if (prepare_pipeline(glob_pipe))
		run_global_pipeline(glob_pipe, env);
	else
		*env->status = 1;
	free_glob_pipe(&glob_pipe);
}

int	main(int argc, char **argv)
{
	char		*line;
	t_env		env;
	extern char	**environ;
	int			status;

	status = 0;
	init_env(&env);
	parse_env(&env, environ);
	env.status = &status;
	increment_shell_level(&env);
	header();
	using_history();
	set_signal();
	while (1)
	{
		line = get_line(&env);
		if (!*line)
		{
			free(line);
			free_doub_array((void **)env.environ);
			continue ;
		}
		parse_and_run(&line, &env);
	}
	rl_clear_history();
	free_doub_array((void **)env.environ);
	return (0);
}
