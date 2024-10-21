/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:01:50 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 22:11:31 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

int	print_file_err(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	return (handle_errors(NULL, NULL, ""));
}

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

/*
	Freeing double array and triple token array, prints error with smart print.

	Return value is 0 if err_msg exists, 1 otherwise.
*/
int	handle_errors_tokens(char **doub_arr, char ***triple_tokens, char *err_msg)
{
	if (doub_arr)
		free_doub_array(doub_arr);
	if (triple_tokens)
		free_triple_tokens(triple_tokens);
	if (err_msg)
		smart_print_err(err_msg);
	return (err_msg == NULL);
}

/*
	Freeing global pipe and double array, prints error with smart print.

	Return value is 0 if err_msg exists, 1 otherwise.
*/
int	handle_errors(t_glob_pipe **glob_pipe, char **doub_arr, char *err_msg)
{
	if (glob_pipe)
		free_glob_pipe(glob_pipe);
	if (doub_arr)
		free_doub_array(doub_arr);
	if (err_msg)
		smart_print_err(err_msg);
	return (err_msg == NULL);
}

void	free_glob_pipe(t_glob_pipe **glob_pipe)
{
	t_glob_pipe	*temp;
	t_glob_pipe	*next;

	if (!glob_pipe || !*glob_pipe)
		return ;
	temp = *glob_pipe;
	while (temp->previous)
		temp = temp->previous;
	while (temp)
	{
		next = temp->next;
		free_doub_array(temp->args);
		free(temp);
		temp = next;
	}
}
