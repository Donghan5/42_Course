/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 16:06:16 by donghank          #+#    #+#             */
/*   Updated: 2024/12/16 22:28:58 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	check the space
	@param
		c: checking char
	@return
		all spaces
*/
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

/*
	check the all case of the empty and comment
	@param
		line: to verify line read by gnl
	@return
		1: success
		0: fail (there's comment or line)
*/
int	is_empty_or_comment(char *line)
{
	if (line == NULL || *line == '\0')
		return (1);
	while (*line)
	{
		if (!ft_isspace(*line))
		{
			if (*line == '#')
				return (1);
			return (0);
		}
		line++;
	}
	return (1);
}

/*
	print error message and exit the program
	@param
		msg: error message
*/
void	handle_error(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	if (!errno)
	{
		ft_putstr_fd("\n", 2);
		exit (1);
	}
	ft_putstr_fd(": ", 2);
	perror(NULL);
	exit (errno);
}
