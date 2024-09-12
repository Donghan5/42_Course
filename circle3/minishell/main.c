/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 00:58:35 by pzinurov          #+#    #+#             */
/*   Updated: 2024/09/12 16:27:36 by donghank         ###   ########.fr       */
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

void run_global_pipeline(t_g_pipe *g, t_env *env)
{
    t_g_pipe	*temp;
    pid_t		pid;
    int			status;
    int			prev_pipe[2];
	int			is_builtin;

	temp = g;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
    while (temp)
    {
        if (!temp->is_exec_ignore)
        {
            if (temp->cmd->next_interaction == PIPE)
            {
                if (pipe(temp->pipe_fds) == -1)
                    exit_error("pipe");
            }
			is_builtin = builtin_check(g->cmd);
			if (!is_builtin)
			{
				pid = fork();
				if (pid == -1)
					exit_error("fork");
			}
            if (is_builtin || (pid == 0))
            {
                setup_operators_child(temp);

                if (prev_pipe[0] != -1)
                {
                    dup2(prev_pipe[0], STDIN_FILENO);
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                }

                if (temp->cmd->next_interaction == PIPE)
                {
                    close(temp->pipe_fds[0]);
                    dup2(temp->pipe_fds[1], STDOUT_FILENO);
                    close(temp->pipe_fds[1]);
                }

                if (!builtin_run(env, temp->cmd))
				{
                    search_path_and_run(temp, env);
                	exit(1);
				}
            }
            if (is_builtin || (pid != 0))
            {
                if (prev_pipe[0] != -1)
                {
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                }
                if (temp->cmd->next_interaction == PIPE)
                {
                    prev_pipe[0] = temp->pipe_fds[0];
                    prev_pipe[1] = temp->pipe_fds[1];
                }
                else
                    prev_pipe[0] = prev_pipe[1] = -1;
                if (!is_builtin && (temp->cmd->next_interaction != PIPE))
                {
                    waitpid(pid, &status, 0);
                    if ((temp->cmd->next_interaction == AND && WEXITSTATUS(status) != 0) ||
                        (temp->cmd->next_interaction == OR && WEXITSTATUS(status) == 0))
                    {
                        break;
                    }
                }
            }
        }
        temp = temp->next;
    }
    while (wait(NULL) > 0);
}

int	main(int argc, char **argv)
{
	char		*line;
	t_command	*(*cmds);
	t_env		env;
	extern char **environ;
	char		*prompt;
	char		**to_free;
	t_g_pipe	*g;

	env.environ = environ;
	parse_env(&env);
	using_history();
	while (1)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
			exit_error("readline");
		if (*line)
			add_history(line);
		to_free = parse(&cmds, line);
		free(line);
		g = cmds_to_global_pipeline(cmds);
		prepare_pipeline(g);
		run_global_pipeline(g, &env);
		free_doub_array((void **) to_free);
		if (cmds)
			free_doub_array((void **) cmds);
	}
	rl_clear_history();
	return (0);
}
