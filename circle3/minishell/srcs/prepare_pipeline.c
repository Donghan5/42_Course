/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/09/24 13:32:46 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_pipeline(t_glob_pipe *glob_pipe, int backwards)
{
	while (glob_pipe && !backwards)
	{
		printf("Name: %s\n", glob_pipe->name);
		printf("Operation: %d\n", glob_pipe->operator);
		printf("Args: ");
		printf("STD: %d %d\n", glob_pipe->redir_io[0], glob_pipe->redir_io[1]);
		printf("Pipes: %d %d\n", glob_pipe->pipe_fds[0], glob_pipe->pipe_fds[1]);
		print_arr(glob_pipe->args);
		printf("\n");
		glob_pipe = glob_pipe->next;
	}
	while (glob_pipe && backwards)
	{
		printf("Name: %s\n", glob_pipe->name);
		printf("Operation: %d\n", glob_pipe->operator);
		printf("Args: ");
		print_arr(glob_pipe->args);
		printf("\n");
		glob_pipe = glob_pipe->previous;
	}
}

void	ft_heredoc(char *stop_word, int fd)
{
	char	*line;

	line = readline("> ");
	while (line)
	{
		if (!ft_strncmp(stop_word, line, ft_strlen(stop_word) + 1))
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	if (line)
		free (line);
}

int	print_file_err(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	return (handle_errors(NULL, NULL, ""));
}

int	setup_redirect(t_glob_pipe *current, t_glob_pipe *next)
{
	int	fd;

	if (!next)
		return (1);
	if (next->operator == REDIRECT_IN)
	{
		fd = open(next->name, O_RDONLY);
		if (fd != -1)
			current->redir_io[0] = fd;
	}
	else if (next->operator == REDIRECT_OUT)
	{
		fd = open(next->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
			current->redir_io[1] = fd;
	}
	else if (next->operator == APPEND_OUT)
	{
		fd = open(next->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd != -1)
			current->redir_io[1] = fd;
	}
	else if (next->operator == HERE_DOC)
	{
		fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
			return (print_file_err(".heredoc"));
		ft_heredoc(next->name, fd);
		close(fd);
		fd = open(".heredoc", O_RDONLY);
		if (fd < 0)
			return (print_file_err(".heredoc"));
		unlink(".heredoc");
		current->redir_io[0] = fd;
	}
	else
		return (1);
	if (fd == -1)
		return (print_file_err(next->name));
	current->files_to_close[current->close_count++] = fd;
	next->is_exec_ignore = 1;
	return (1);
}

int	prepare_pipeline(t_glob_pipe *glob_pipe)
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
		while ((current->operator == REDIRECT_EXPECTED) && next
			&& (next->operator == REDIRECT_IN
				|| next->operator == REDIRECT_OUT
				|| next->operator == APPEND_OUT
				|| next->operator == HERE_DOC))
		{
			if (!setup_redirect(current, next))
				return (0);
			next = next->next;
		}
		if (current->operator == PIPE)
		{
			if (pipe(current->pipe_fds) == -1)
				return (handle_errors(NULL, NULL, "minishell: pipe"));
		}
		current = next;
	}
	return (1);
}
