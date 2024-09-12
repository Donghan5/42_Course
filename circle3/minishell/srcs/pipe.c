/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:25:18 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 14:59:33 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setup_pipes(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

int is_readable(const char *filename) {
	struct stat buffer;

	if (stat(filename, &buffer) == 0)
	{
		if (buffer.st_mode & R_OK)
			return (1);
	}
	return (0);
}

void	ft_heredoc(char *stop_word, int fd)
{
	char	*line;

	line = readline("> ");
	while (line)
	{
		if (ft_strncmp(stop_word, line, ft_strlen(line) + 1) == 0)
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	if (line)
		free (line);
}

void	handle_redirection(t_command *cmd, t_command *cmd2)
{
	int	fd;

	if (cmd->next_interaction == REDIRECT_IN)
	{
		fd = open(cmd2->exec_name, O_RDONLY);
		if (fd < 0)
		{
			perror(cmd->exec_name);
			return ;
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->next_interaction == REDIRECT_OUT)
	{
		fd = open(cmd->exec_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd < 0)
			exit_error("open");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (cmd->next_interaction == APPEND_OUT)
	{
		fd = open(cmd->exec_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			exit_error("open");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (cmd->next_interaction == HERE_DOC)
	{
		fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
		{
			perror(".heredoc");
			return ;
		}
		ft_heredoc(".heredoc", fd);
		close(fd);
		fd = open(".heredoc", O_RDONLY);
		unlink(".heredoc");
		// return (ret);
	}
}
