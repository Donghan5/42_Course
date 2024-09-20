/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:36:45 by donghank          #+#    #+#             */
/*   Updated: 2024/09/20 17:04:33 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to extract inside of the quote
// static int	cmd_cnt(char *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd[i] && ft_iswhitespace((int)cmd[i]))
// 		i++;
// 	while (cmd[i] && !ft_iswhitespace((int)cmd[i]))
// 		i++;
// 	while (cmd[i])
// 	{
// 		while (cmd[i] && ft_iswhitespace((int)cmd[i]))
// 			i++;
// 		if (cmd[i] == '-')
// 		{
// 			i++;
// 			while (cmd[i] && (ft_isalpha((int)cmd[i]) || \
// 			cmd[i] == '-' || ft_isdigit((int)cmd[i])))
// 				i++;
// 		}
// 		else
// 			break ;
// 	}
// 	return (i);
// }

// to treat expander " or ' or $
// tmp_expanded = expanded string which contain command
// expanded = expanded string without command
// should be connect with the parse part
char	*expander(char *cmd, t_env *env)
{
	int		cmd_len;
	int		cmd_end;
	char	*expanded;

	if (cmd == NULL || env == NULL || env->environ == NULL)
		return (ft_strdup(""));
	cmd_len = get_env_parse_len(cmd, env->environ);
	if (cmd_len == 0)
		return (ft_strdup(""));
	expanded = (char *)malloc(sizeof(char) * (cmd_len + 1));
	if (!expanded)
		return (NULL);
	copy_strings(cmd, expanded, env->environ);
	return (expanded);
}
