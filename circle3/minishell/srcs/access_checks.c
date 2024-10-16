/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 19:09:29 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/16 20:22:59 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	can_access(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	if (access(path, W_OK) == 0)
		return (1);
	if (access(path, R_OK) == 0)
		return (1);
	return (0);
}

int	does_exist(char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) == 0)
		return (1);
	return (0);
}

int	is_directory(char *path)
{
	DIR	*dir;

	if (*path != '/' && *path != '.')
		return (0);
	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}
