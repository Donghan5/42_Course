/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:01:50 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/17 14:12:03 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_glob_pipe(t_glob_pipe **glob_pipe)
{
	t_glob_pipe	*temp;
	t_glob_pipe *next;

	if (!glob_pipe || !*glob_pipe)
		return ;
	temp = *glob_pipe;
	while (temp)
	{
		next = temp->next;
		// fprintf(stderr, "freed %s\n", temp->name);
		free_doub_array((void **) temp->args);
		free(temp);
		temp = next;
	}
}