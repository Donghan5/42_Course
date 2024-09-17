/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/17 16:55:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history,
// printf, malloc, free, write, access, open, read,
// close, fork, wait, waitpid, wait3, wait4, signal,
// sigaction, sigemptyset, sigaddset, kill, exit,
// getcwd, chdir, stat, lstat, fstat, unlink, execve,
// dup, dup2, pipe, opendir, readdir, closedir,
// strerror, perror, isatty, ttyname, ttyslot, ioctl,
// getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
// tgetnum, tgetstr, tgoto, tputs

void run_global_pipeline(t_glob_pipe *cmds_start, t_env *env)
{
	t_glob_pipe	*temp_cmd;
	pid_t		pid;
	int			status;
	int			prev_pipe_read = -1;
	int			is_builtin;

	temp_cmd = cmds_start;
	status = 0;
	while (temp_cmd)
	{
		if (!temp_cmd->is_exec_ignore && temp_cmd->name)
		{
			is_builtin = builtin_check(temp_cmd);
			if (!is_builtin)
			{
				pid = fork();
				if (pid == -1)
					exit_error("fork");
			}
			if (is_builtin || (pid == 0))
			{
				if (temp_cmd->redir_io[0] != STDIN_FILENO)
					dup2(temp_cmd->redir_io[0], STDIN_FILENO);
				else if (prev_pipe_read != -1)
				{
					dup2(prev_pipe_read, STDIN_FILENO);
					close(prev_pipe_read);
				}
				if (temp_cmd->redir_io[1] != STDOUT_FILENO)
					dup2(temp_cmd->redir_io[1], STDOUT_FILENO);
				else if (temp_cmd->operator == PIPE)
				{
					close(temp_cmd->pipe_fds[0]);
					dup2(temp_cmd->pipe_fds[1], STDOUT_FILENO);
				}
				close_fds(temp_cmd);
				// fprintf(stderr, "%s: pipe_fds %d %d, stdio %d %d, prev pipe %d\n", temp_cmd->name, temp_cmd->pipe_fds[0], temp_cmd->pipe_fds[1], temp_cmd->redir_io[0], temp_cmd->redir_io[1], prev_pipe_read);
				if (!builtin_run(env, temp_cmd, &status))
				{
					search_path_and_run(temp_cmd, env);
					exit(1);
				}
				if (!is_builtin)
					exit(0);
			}
			if (!is_builtin)
			{
				if (prev_pipe_read != -1)
					close(prev_pipe_read);
				if (temp_cmd->operator == PIPE)
				{
					close(temp_cmd->pipe_fds[1]);
					prev_pipe_read = temp_cmd->pipe_fds[0];
				}
				else
					prev_pipe_read = -1;
				close_fds(temp_cmd);
				if (temp_cmd->operator != PIPE)
				{
					waitpid(pid, &status, 0);
					if ((temp_cmd->operator == AND && WEXITSTATUS(status) != 0) ||
						(temp_cmd->operator == OR && WEXITSTATUS(status) == 0))
					break;
				}
			}
		}
		temp_cmd = temp_cmd->next;
	}
	while (wait(NULL) > 0);
}

int	main(int argc, char **argv)
{
	char		*line;
	t_env		env;
	extern char	**environ;
	char		*prompt;
	t_glob_pipe	*glob_pipe;

	env.environ = environ;
	parse_env(&env);
	header();
	using_history();
	while (1)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		handle_eof(line);
		if (!line)
			exit_error("readline");
		if (*line)
			add_history(line);
		if (!*line || !parse(&glob_pipe, line))
		{
			free(line);
			continue ;
		}
		free(line);
		if (prepare_pipeline(glob_pipe))
			run_global_pipeline(glob_pipe, &env);
		free_glob_pipe(&glob_pipe);
	}
	rl_clear_history();
	return (0);
}
