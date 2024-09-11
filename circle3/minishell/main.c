/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/11 12:19:57 by donghank         ###   ########.fr       */
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


// void	execute_commmands(t_command	**cmds, t_env *env)
// {
// 	int	i;

// 	i = 0;
// 	while (cmds[i])
// 	{
// 		i++;
// 	}

// 	if (!buildin_check(env, cmd))
// 		search_path_and_run(cmd, env);
// }

int	main(int argc, char **argv)
{
	char		*line;
	t_command	*(*cmds);
	t_env		env;
	extern char **environ;
	char		*prompt;
	int			i;
	char		**to_free;
	int			fds[2];
	int			pipe_fd[2];
	int			temp_pipe_read;

	env.environ = environ;
	parse_env(&env);
	// printf("%s\n", get_value_for_name(env.environ_name_value, "HOME"));
	using_history();
	set_signal();
	int tempin = dup(STDIN_FILENO);
	while (1)
	{
		i = 0;
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
			exit_error("readline");
		if (*line)
			add_history(line);
		to_free = parse(&cmds, line);
		free(line);
		temp_pipe_read = STDIN_FILENO;
		while (cmds && cmds[i])
		{
			fds[0] = temp_pipe_read;
			fds[1] = STDOUT_FILENO;
			if (cmds[i]->next_interaction == PIPE)
			{
				if (pipe(pipe_fd) == -1)
					exit_error("pipe");
				fds[1] = pipe_fd[1];
			}
			// handle_redirection(cmds[0], cmds[1]);
			// printf("cmd=%s, iner=%d\n", cmds[i]->exec_name, cmds[i]->next_interaction);
			if (!buildin_check(&env, cmds[i]))
				search_path_and_run(cmds[i], &env, fds);
			if (temp_pipe_read != STDIN_FILENO)
				close(temp_pipe_read);
			if (cmds[i]->next_interaction == PIPE)
			{
				close(pipe_fd[1]);
				temp_pipe_read = pipe_fd[0];
			}
			else
				temp_pipe_read = STDIN_FILENO;
			i++;
		}
		free_doub_array((void **) to_free);
		if (cmds)
			free_doub_array((void **) cmds);
		dup2(tempin, STDIN_FILENO);
	}
	rl_clear_history();
	return (0);
}
