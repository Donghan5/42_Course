/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_global_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:10:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/10/18 02:23:56 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

/*
	Running whole global pipeline of commands
*/
void pipeline_cycle(t_glob_pipe *t, int *prev, t_env *e)
{
    int built;
    pid_t pid;
    pid_t *child_pids = NULL;
    int child_count = 0;
    int skip_next = 0;

    while (t)
    {
        if (skip_next)
        {
            skip_next = 0;
            if (t->priority > 0)
            {
                int current_priority = t->priority;
                while (t && t->priority >= current_priority)
                    t = t->next;
                continue;
            }
            while (t && t->op == PIPE)
                t = t->next;
            if (t)
                t = t->next;
            continue;
        }

        no_execs(t, e, prev);
        if (!t->is_exec_ignore)
        {
            built = builtin_check(t);
            pid = -1;
            if (t->op == PIPE || *prev != -1 || !built)
            {
                pid = fork();
                if (pid == -1)
                {
                    close_fds(t, 1, 0);
                    perror("fork");
                    break;
                }
                else if (pid > 0)
                {
                    child_count++;
                    child_pids = realloc(child_pids, child_count * sizeof(pid_t));
                    child_pids[child_count - 1] = pid;
                }
            }

            if (pid == 0)
                child_process(prev, t, built, e);
            else if (pid > 0)
                parent_process(t, prev, e, pid);
            else
                builtin_no_process(t, e);
        }

        if (t->op != PIPE || !t->next)
        {
            for (int i = 0; i < child_count; i++)
            {
                int status;
                waitpid(child_pids[i], &status, 0);
                if (i == child_count - 1)
                {
                    if (WIFEXITED(status))
                        e->sts = WEXITSTATUS(status);
                    else
                        e->sts = 1;
                }
            }
            free(child_pids);
            child_pids = NULL;
            child_count = 0;
            if ((t->op == AND && e->sts != 0) || (t->op == OR && e->sts == 0))
                skip_next = 1;
        }

        t = t->next;
    }

    if (child_pids)
        free(child_pids);
}


void pipeline_cycle2(t_glob_pipe *t, int *prev, t_env *e)
{
	int built;
	pid_t pid;
	pid_t *child_pids = NULL;
	int child_count = 0;
	int skip_next = 0;
	static int priority;

	while (t)
	{
		if (skip_next)
		{
			skip_next = 0;
			if (t->priority == priority)
				t = t->next;
			while (t && t->priority != priority)
			{
				t = t->next;
			}

			continue ;
		}

		if (t->priority > 0)
		{
			pid_t subshell_pid = fork();
			if (subshell_pid == 0)
			{
				int subshell_prev = -1;
				t_glob_pipe *subshell_cmd = t;
				int current_priority = t->priority;
				while (subshell_cmd && subshell_cmd->priority >= current_priority)
				{
					pipeline_cycle2(subshell_cmd, &subshell_prev, e);
					while (subshell_cmd->next && subshell_cmd->next->op == PIPE)
						subshell_cmd = subshell_cmd->next;
					subshell_cmd = subshell_cmd->next;
				}
				exit(e->sts);
			}
			else if (subshell_pid > 0)
			{
				int status = 0;
				waitpid(subshell_pid, &status, 0);
				if (WIFEXITED(status))
					e->sts = WEXITSTATUS(status);
				else
					e->sts = 1;
				int current_priority = t->priority;
				while (t->next && t->next->priority >= current_priority)
					t = t->next;
			}
			else
			{
				perror("fork");
				e->sts = 1;
			}
		}
		else
		{
			no_execs(t, e, prev);
			if (!t->is_exec_ignore)
			{
				built = builtin_check(t);
				pid = -1;
				if (t->op == PIPE || *prev != -1 || !built)
				{
					pid = fork();
					if (pid == -1)
					{
						close_fds(t, 1, 0);
						perror("fork");
						break ;
					}
					else if (pid > 0)
					{
						child_count++;
						child_pids = realloc(child_pids, child_count * sizeof(pid_t));
						child_pids[child_count - 1] = pid;
					}
				}

				if (pid == 0)
					child_process(prev, t, built, e);
				else if (pid > 0)
					parent_process(t, prev, e, pid);
				else
					builtin_no_process(t, e);
			}

			if (t->op != PIPE || !t->next)
			{
				for (int i = 0; i < child_count; i++)
				{
					int status = 0;
					waitpid(child_pids[i], &status, 0);
					if (i == child_count - 1)
					{
						if (WIFEXITED(status))
							e->sts = WEXITSTATUS(status);
						else
							e->sts = 1;
					}
				}
				free(child_pids);
				child_pids = NULL;
				child_count = 0;
			}
		}
		if (t->next)
		{
			if ((t->op == AND && e->sts != 0) || (t->op == OR && e->sts == 0))
				skip_next = 1;
		}
		priority = t->priority;
		t = t->next;
	}
	if (child_pids)
		free(child_pids);
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
	pipeline_cycle2(temp_cmd, &prev_pipe, env);
	wait_background_processes();
}
