/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimdonghan <kimdonghan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:36:45 by donghank          #+#    #+#             */
/*   Updated: 2024/10/11 23:18:58 by kimdonghan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// to treat expander " or ' or $
// tmp_expanded = expanded string which contain command
// expanded = expanded string without command
char	*expander(char *cmd, t_env *env)
{
	int		cmd_len;
	char	*expanded;

	cmd_len = get_env_parse_len(cmd, env);
	expanded = (char *)malloc(sizeof(char) * (cmd_len + 1));
	if (!expanded)
		return (NULL);
	copy_strings(cmd, expanded, env);
	return (expanded);
}
