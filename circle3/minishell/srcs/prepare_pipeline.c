/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/10/25 18:31:29 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	setup_redirect(t_glob_pipe *current, t_glob_pipe *next, t_env *env)
{
	int	fd;

	fd = 0;
	if (next->op == REDIRECT_IN)
	{
		fd = open(next->name, O_RDONLY);
		current->redir_io[0] = fd;
	}
	else if (next->op == REDIRECT_OUT)
	{
		fd = open(next->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		current->redir_io[1] = fd;
	}
	else if (next->op == APPEND_OUT)
	{
		fd = open(next->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		current->redir_io[1] = fd;
	}
	else if ((next->op == HERE_DOC) && !setup_heredoc(current, next, &fd, env))
		return (0);
	if (fd == -1)
		return (print_file_err(next->name));
	current->files_to_close[current->close_count++] = fd;
	next->is_exec_ignore = 1;
	return (1);
}

int	setup_operator(t_glob_pipe *current, t_glob_pipe **next, t_env *env)
{
	while ((current->op == REDIRECT_EXPECTED || current->op == REDIR_PIPE)
		&& (*next) && ((*next)->op == REDIRECT_IN || (*next)->op == REDIRECT_OUT
			|| (*next)->op == APPEND_OUT || (*next)->op == HERE_DOC))
	{
		if ((!current->is_exec_ignore || (*next)->op == HERE_DOC)
			&& !setup_redirect(current, (*next), env))
		{
			current->is_exec_ignore = 1;
			if ((*next)->op != HERE_DOC)
				env->sts = 1;
			else
				return (0);
		}
		(*next)->is_exec_ignore = 1;
		(*next) = (*next)->next;
	}
	if (current->op == PIPE || current->op == REDIR_PIPE)
	{
		current->op = PIPE;
		if (current->is_exec_ignore)
			current->op = NO_EXEC_PIPE;
		if (pipe(current->pipe_fds) == -1)
			return (handle_errors(NULL, NULL, "minishell: pipe"));
	}
	return (1);
}

void	close_fds_pipeline(t_glob_pipe *glob_pipe)
{
	while (glob_pipe)
	{
		smart_close(glob_pipe->pipe_fds[0]);
		smart_close(glob_pipe->pipe_fds[1]);
		smart_close(glob_pipe->redir_io[0]);
		smart_close(glob_pipe->redir_io[1]);
		close_fds(glob_pipe, 0, 0);
		glob_pipe = glob_pipe->next;
	}
}

int	prepare_pipeline(t_glob_pipe *glob_pipe, t_env *env)
{
	t_glob_pipe	*current;
	t_glob_pipe	*next;

	current = glob_pipe;
	while (current)
	{
		current->redir_io[0] = STDIN_FILENO;
		current->redir_io[1] = STDOUT_FILENO;
		current->is_exec_ignore = 0;
		next = current->next;
		if (!setup_operator(current, &next, env))
		{
			close_fds_pipeline(glob_pipe);
			return (0);
		}
		if (!current->op && !current->name)
			current->is_exec_ignore = 1;
		current = next;
	}
	return (1);
}
