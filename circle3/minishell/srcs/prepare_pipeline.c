/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:35 by donghank          #+#    #+#             */
/*   Updated: 2024/10/13 20:53:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_heredoc(char *stop_word, int fd)
{
	char	*line;

	signal(SIG_INT, sigint_heredoc);
	line = readline("> ");
	while (1)
	{
		if (!line)
		{
			printf("minishell: warning: here-document \
delimited by end-of-file (wanted `%s')\n", stop_word);
			break ;
		}
		if (!ft_strncmp(stop_word, line, ft_strlen(stop_word) + 1))
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	if (line)
		free (line);
	signal(SIGINT, SIG_DFL);
}

int	setup_heredoc(t_glob_pipe *current, t_glob_pipe *next, int *fd)
{
	*fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
		return (print_file_err(".heredoc"));
	ft_heredoc(next->name, *fd);
	close(*fd);
	*fd = open(".heredoc", O_RDONLY);
	if (*fd < 0)
		return (print_file_err(".heredoc"));
	unlink(".heredoc");
	current->redir_io[0] = *fd;
	return (1);
}

int	setup_redirect(t_glob_pipe *current, t_glob_pipe *next)
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
	else if ((next->op == HERE_DOC) && !setup_heredoc(current, next, &fd))
		return (0);
	if (fd == -1)
		return (print_file_err(next->name));
	current->files_to_close[current->close_count++] = fd;
	next->is_exec_ignore = 1;
	return (1);
}

int	setup_operator(t_glob_pipe *current, t_glob_pipe **next, t_env *env)
{
	while ((current->op == REDIRECT_EXPECTED
			|| current->op == REDIR_PIPE)
		&& (*next) && ((*next)->op == REDIRECT_IN
			|| (*next)->op == REDIRECT_OUT
			|| (*next)->op == APPEND_OUT
			|| (*next)->op == HERE_DOC))
	{
		if ((!current->is_exec_ignore || (*next)->op == HERE_DOC)
			&& !setup_redirect(current, (*next)))
		{
			current->is_exec_ignore = 1;
			env->sts = 1;
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
			return (0);
		current = next;
	}
	return (1);
}
