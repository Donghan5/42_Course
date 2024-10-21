/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 22:10:36 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

/*
	Closing only real fds
*/
void	smart_close(int fd)
{
	if (fd != -1 && fd > 2)
		close(fd);
}

/*
	Wait for all background processes, no status update
*/
void	wait_background_processes(void)
{
	int	status;

	while (wait(&status) > 0)
		(void)status;
}

int	process_picker(t_glob_pipe *t, t_env *e, int *prev)
{
	pid_t	pid;
	int		built;

	built = builtin_check(t);
	pid = -1;
	if (t->op == PIPE || t->priority > 0 || *prev != -1 || !built)
		pid = fork();
	if ((t->op == PIPE || t->priority > 0 || *prev != -1 || !built)
		&& (pid == -1))
		return (close_fds(t, 1, 0), perror("fork"), 0);
	if (pid == 0)
		child_process(prev, t, built, e);
	else if (pid > 0)
		parent_process(t, prev, e, pid);
	else
		builtin_no_process(t, e);
	return (1);
}

/*
	Running whole global pipeline of commands
*/
void	pipeline_cycle(t_glob_pipe *t, int *prev, t_env *e)
{
	skipper(NULL, NULL, 2, -1);
	while (t)
	{
		t = skipper(t, e, 1, -1);
		if (!t)
			break ;
		no_execs(t, e, prev);
		if (!t->is_exec_ignore && !process_picker(t, e, prev))
			return ;
		if (t->op != PIPE || !t->next)
		{
			children_manager(0, e, 1, 0);
			skipper(t, e, 0, -1);
		}
		skipper(NULL, NULL, 0, t->priority);
		t = t->next;
	}
	children_manager(0, NULL, 0, 1);
}

/*
	Parent outer shell to run all commands
*/
void	run_global_pipeline(t_glob_pipe **cmds_start, t_env *env)
{
	t_glob_pipe	*temp_cmd;
	int			prev_pipe;

	prev_pipe = -1;
	temp_cmd = *cmds_start;
	pipeline_cycle(temp_cmd, &prev_pipe, env);
	wait_background_processes();
}
