/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/26 16:27:12 by pzinurov         ###   ########.fr       */
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
		exit(env->status);
	}
	if (*line)
		add_history(line);
	return (line);
}

int	is_lex_error(char ***tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (1);
	while (tokens[i])
	{
		if (is_tokens_error(tokens[i], tokens[i + 1], i))
			return (compound_token_error(tokens[i], tokens[i + 1], i, NULL), 1);
		i++;
	}
	return (0);
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
		env->status = 2;
		return ;
	}
	free_triple_tokens(tokens);
	if (prepare_pipeline(glob_pipe, env))
		run_global_pipeline(glob_pipe, env);
	else
		env->status = 1;
	free_glob_pipe(&glob_pipe);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_env		env;

	parse_env(&env, envp);
	increment_shell_level(&env);
	// header();
	using_history();
	set_signal();
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
