/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:51:03 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 20:48:56 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(char *perror_message)
{
	perror(perror_message);
	rl_clear_history();
	if (errno)
		exit(errno);
	exit(1);
}

int	is_numeric(char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
