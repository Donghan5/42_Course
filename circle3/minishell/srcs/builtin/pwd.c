/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:25:41 by donghank          #+#    #+#             */
/*   Updated: 2024/09/16 11:27:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// built-in function pwd --> print the path
int	pwd_check(t_command *cmd)
{
	int	buf;

	if (!ft_strncmp(cmd->exec_name, "pwd", ft_strlen(cmd->exec_name)))
	{
		printf("%s\n", getcwd(NULL, buf));
		return (RUN);
	}
	return (NOT_RUN);
}
