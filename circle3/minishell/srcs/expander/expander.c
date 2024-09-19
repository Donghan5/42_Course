/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:36:45 by donghank          #+#    #+#             */
/*   Updated: 2024/09/20 01:21:06 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_cnt(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_iswhitespace((int)cmd[i]) == 1)
			return (i);
		i++;
	}
	return (0);
}

// to treat expander " or ' or $
// tmp_expanded = expanded string which contain command
// expanded = expanded string without command
char	*expander(char *cmd, char **envp)
{
	int		cmd_len;
	char	*tmp_expanded;
	char	*expanded;

	cmd_len = get_env_parse_len(cmd, envp);
	tmp_expanded = (char *)malloc(sizeof(char) * (cmd_len + 1));
	if (!tmp_expanded)
		return (NULL);
	copy_strings(cmd, tmp_expanded, envp);
	free(cmd);
	printf("cmd_cnt: %d\n", cmd_cnt(cmd));
	expanded = ft_substr(tmp_expanded, cmd_cnt(tmp_expanded) + 1, cmd_len);
	return (expanded);
}
