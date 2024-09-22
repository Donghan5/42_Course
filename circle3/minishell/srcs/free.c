/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:01:50 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/21 15:54:46 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Freeing global pipe and double array, prints error with smart print.

	Return value is 0 if err_msg exists, 1 otherwise.
*/

void	free_triple_tokens(char ***arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (arr[i][0])
			free(arr[i][0]);
		if (arr[i][1])
			free(arr[i][1]);
		free (arr[i]);
		i++;
	}
	if (arr)
		free (arr);
}

int		handle_errors_tokens(char **doub_arr, char ***triple_tokens, char *err_msg)
{
	if (doub_arr)
		free_doub_array((void **) doub_arr);
	if (triple_tokens)
		free_triple_tokens(triple_tokens);
	if (err_msg)
		smart_print_err(err_msg);
	return (err_msg == NULL);
}

int		handle_errors(t_glob_pipe **glob_pipe, char **doub_arr, char *err_msg)
{
	if (glob_pipe)
		free_glob_pipe(glob_pipe);
	if (doub_arr)
		free_doub_array((void **) doub_arr);
	if (err_msg)
		smart_print_err(err_msg);
	return (err_msg == NULL);
}

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
		free_doub_array((void **) temp->args);
		free(temp);
		temp = next;
	}
}