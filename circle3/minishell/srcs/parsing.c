/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/09/10 13:23:53 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse(t_command **(*cmds), char *line)
{
	int			i;
	int			j;
	int			cmds_counter;
	char		**whitespaced;

	i = 0;
	cmds_counter = 0;
	whitespaced = ft_whitesplit(line);
	if (!whitespaced)
		return (NULL);
	while(whitespaced[i])
	{
		if (fill_interaction(NULL, whitespaced[i]))
			cmds_counter++;
		i++;
	}
	if (whitespaced && (i > 0))
		cmds_counter++;
	if (!cmds_counter)
		return (free_doub_array((void **) whitespaced), NULL);
	(*cmds) = malloc(sizeof(t_command *) * (cmds_counter + 1));
	if (!(*cmds))
		return (free_doub_array((void **) whitespaced), NULL);
	i = 0;
	j = 0;
	(*cmds)[j] = malloc(sizeof(t_command));
	if (!(*cmds)[j])
		return (free_doub_array((void **) whitespaced), NULL);
	while (whitespaced[i])
	{
		cmds_counter = 0;
		memset((*cmds)[j], 0, sizeof (t_command));
		while (whitespaced[i] && !fill_interaction(NULL, whitespaced[i]))
		{
			cmds_counter++;
			if (!whitespaced[i + 1])
				break;
			i++;
		}
		if (whitespaced[i] && (fill_interaction(NULL, whitespaced[i])) && !whitespaced[i + 1])
		{
			printf("ARGS BAD\n");
				return (free_doub_array((void **) whitespaced), NULL); //add free for cmds
		}
		if (whitespaced[i] && (fill_interaction((*cmds)[j], whitespaced[i])) || (!whitespaced[i + 1]))
		{
			if (!whitespaced[i + 1])
				fill_cmd(whitespaced, (*cmds)[j], cmds_counter, i + 1 - cmds_counter);
			else
				fill_cmd(whitespaced, (*cmds)[j], cmds_counter, i - cmds_counter);
			if (!((*cmds))[j]->args)
			{
				printf("MEM BAD\n");
				return (free_doub_array((void **) whitespaced), NULL); //add free for cmds
			}
			if (whitespaced[i + 1] && fill_interaction(NULL, whitespaced[i + 1]))
			{
				printf("ARGS BAD\n");
					return (free_doub_array((void **) whitespaced), NULL); //add free for cmds
			}
			if (whitespaced[i + 1])
			{
				(*cmds)[j + 1] = malloc(sizeof(t_command));
				if (!(*cmds)[j + 1])
					return (free_doub_array((void **) whitespaced), NULL); //add free for cmds
			}
			j++;
			i++;
		}
	}
	(*cmds)[j] = NULL;
	return (whitespaced);
}

void	parse_env(t_env *env)
{
	int		i;
	char	**temp;

	i = 0;
	while (env->environ[i])
		i++;
	// env->environ_name_value = malloc(sizeof(t_env *) * (i + 1));
	// if (!env->environ_name_value)
	// 	exit_error("malloc");
	// i = 0;
	// while (env->environ[i])
	// {
	// 	temp = ft_split(env->environ[i], '=');
	// 	env->environ_name_value[i].name = temp[0];
	// 	env->environ_name_value[i].value = temp[1];
	// 	i++;
	// }
	// env->environ_name_value[i].name = NULL;
	// env->environ_name_value[i].value = NULL;
}
