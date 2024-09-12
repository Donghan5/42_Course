/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 14:41:08 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_pipeline(t_g_pipe *g, int backwards)
{
	while (g && !backwards)
	{
		printf("~%s~\n", g->cmd->exec_name);
		g = g->next;
	}
	while (g && backwards)
	{
		printf("~%s~\n", g->cmd->exec_name);
		g = g->previous;
	}
	
}

t_g_pipe	*cmd_to_line(t_command *cmd, t_g_pipe *prev)
{
	t_g_pipe	*new_line;

	new_line = malloc(sizeof (t_g_pipe));
	if (!new_line)
		return (NULL);
	new_line->cmd = cmd;
	new_line->next = NULL;
	new_line->previous = NULL;
	new_line->is_exec_ignore = 0;
	new_line->close_count = 0;
	if (prev)
	{
		new_line->previous = prev;
		prev->next = new_line;
	}
	new_line->standard_io[0] = dup(STDIN_FILENO);
	new_line->standard_io[1] = dup(STDOUT_FILENO);
	return (new_line);
}

t_g_pipe	*cmds_to_global_pipeline(t_command **cmds)
{
	int	i;
	t_g_pipe	*start;
	t_g_pipe	*temp;
	t_g_pipe	*previous;

	i = 0;
	previous = NULL;
	while (cmds[i])
	{
		temp = cmd_to_line(cmds[i], previous);
		if (temp == NULL)
		{
			printf("temp = %s\n", temp->cmd->exec_name);
			return (NULL);
		}
		if (!previous)
			start = temp;
		previous = temp;
		i++;
	}
	return (start);
}

int	setup_pipe(t_g_pipe *temp)
{
	t_g_pipe	*next;

	next = temp->next;
	if ((temp->cmd->next_interaction == PIPE) && next)
	{
		if (pipe(temp->pipe_fds) == -1)
			return (0);
		temp->standard_io[1] = temp->pipe_fds[1];
		temp->standard_io[0] = temp->pipe_fds[0];
		temp->files_to_close[temp->close_count++] = temp->pipe_fds[1];
		next->files_to_close[next->close_count++] = temp->pipe_fds[0];
	}
	return (1);
}

int	setup_redirect_in(t_g_pipe *temp)
{
	t_g_pipe	*next;
	int					fd;

	next = temp->next;
	if (temp->cmd->next_interaction == REDIRECT_IN && next)
	{
		fd = open(next->cmd->exec_name, O_RDONLY);
		if (fd == -1)
			return (0);
		temp->standard_io[0] = fd;
		temp->files_to_close[temp->close_count++] = fd;
		next->is_exec_ignore = 1;
	}
	return (1);
}

int	setup_redirect_out(t_g_pipe *temp)
{
	t_g_pipe	*next;
	int					fd;

	next = temp->next;
	if (temp->cmd->next_interaction == REDIRECT_OUT && next)
	{
		fd = open(next->cmd->exec_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (0);
		temp->standard_io[1] = fd;
		temp->files_to_close[temp->close_count++] = fd;
		next->is_exec_ignore = 1;
	}
	return (1);
}

int setup_append_out(t_g_pipe *temp)
{
	t_g_pipe	*next;
	int					fd;

	next = temp->next;
	if (temp->cmd->next_interaction == APPEND_OUT && next)
	{
		fd = open(next->cmd->exec_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (0);
		temp->standard_io[1] = fd;
        temp->files_to_close[temp->close_count++] = fd;
		next->is_exec_ignore = 1;
	}
	return (1);
}

int	prepare_pipeline(t_g_pipe *g)
{
	t_g_pipe	*temp;

	temp = g;
	while (temp)
	{
		if (!temp->is_exec_ignore)
		{			
			temp->standard_io[0] = STDIN_FILENO;
			temp->standard_io[1] = STDOUT_FILENO;
			if (!(setup_pipe(temp) && setup_redirect_in(temp) && setup_redirect_out(temp) && setup_append_out(temp)))
				exit_error("open");
		}
		temp = temp->next;
	}
	
	return (1);
}
