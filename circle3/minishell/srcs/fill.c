/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:26:46 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 14:58:06 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_cmd(char **whitespaced, t_command *cmd, int n, int start_index)
{
	int	i;

	i = 0;
	cmd->args = malloc (sizeof (char *) * (n + 1));
	if (!cmd->args)
		return ;
	while (i < n)
	{
		cmd->args[i] = whitespaced[start_index + i];
		i++;
	}
	cmd->args[i] = NULL;
	cmd->exec_name = cmd->args[0];
}

int	fill_interaction(t_command *cmd, char *word)
{
	int	interaction;

	interaction = 0;
	if (!ft_strncmp(word, "||", 3))
		interaction = OR;
	else if (!ft_strncmp(word, "&&", 3))
		interaction = AND;
	else if (!ft_strncmp(word, ">>", 3))
		interaction = APPEND_OUT;
	else if (!ft_strncmp(word, "<<", 3))
		interaction = HERE_DOC;
	else if ((word[0] == '|') && (ft_strlen(word) == 1))
		interaction = PIPE;
	else if ((word[0] == '>') && (ft_strlen(word) == 1))
		interaction = REDIRECT_OUT;
	else if ((word[0] == '<') && (ft_strlen(word) == 1))
		interaction = REDIRECT_IN;
	if (interaction && cmd)
		cmd->next_interaction = interaction;
	return (interaction);
}
