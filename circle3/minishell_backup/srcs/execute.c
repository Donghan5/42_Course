/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/10/18 13:39:55 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	Closes pipe fds from the previous and/or next commands.
*/
int	close_prev_pipes(t_glob_pipe *cmd, int previous_only)
{
	t_glob_pipe	*current;

	current = cmd;
	while (current->previous)
		current = current->previous;
	while (current)
	{
		if (current->op == PIPE)
		{
			if (current != cmd && current != cmd->previous)
			{
				smart_close(current->pipe_fds[0]);
				smart_close(current->pipe_fds[1]);
			}
			else if (previous_only)
				return (1);
		}
		current = current->next;
	}
	return (1);
}

/*
	Closes all file fds from the command given.
	Accepts arguments to call 	close_prev_pipes(glob_pipe, pipes_prev_only)
*/
void	close_fds(t_glob_pipe *glob_pipe, int close_pipes, int pipes_prev_only)
{
	int	i;

	i = 0;
	while (i < glob_pipe->close_count)
	{
		smart_close(glob_pipe->files_to_close[i]);
		i++;
	}
	if (close_pipes)
		close_prev_pipes(glob_pipe, pipes_prev_only);
}

void	compound_error_exit(t_glob_pipe *cmds, t_env *env,
	int exit_code, char *msg)
{
	write(2, cmds->name, ft_strlen(cmds->name));
	if ((exit_code == 127)
		&& (cmds->name[0] == '/'
			|| ((cmds->name[0] == '.') && (cmds->name[1] == '/'))))
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(msg, 2);
	free_glob_pipe(&cmds);
	free_doub_array(env->environ);
	rl_clear_history();
	exit(exit_code);
}

int	path_search(char **splitted, t_glob_pipe *cmds, t_env *env, int *is_found)
{
	char	*full_path;
	int		i;

	i = 0;
	while (splitted && splitted[i])
	{
		full_path = triple_strjoin(splitted[i], "/", cmds->name);
		if (!full_path)
			return (handle_errors(&cmds, splitted, NULL),
				free_doub_array(env->environ), exit_error("malloc"), 0);
		if (is_directory(full_path))
			compound_error_exit(cmds, env, 126, ": Is a directory\n");
		if (does_exist(full_path) && !can_access(full_path))
			compound_error_exit(cmds, env, 126, ": Permission denied\n");
		if (does_exist(full_path) && can_access(full_path))
		{
			free_doub_array(splitted);
			*is_found = 1;
			execve(full_path, cmds->args, env->environ);
			return (free_doub_array(env->environ), exit_error(""), 0);
		}
		free(full_path);
		i++;
	}
	return (0);
}

void	search_path_and_run(t_glob_pipe *cmds, t_env *env)
{
	char	*path;
	char	**splitted;
	int		is_found;

	is_found = 0;
	if (!*cmds->name)
		compound_error_exit(cmds, env, 127, ": command not found\n");
	path = getenv_value("PATH", env->environ);
	splitted = ft_split(path, ':');
	path_search(splitted, cmds, env, &is_found);
	free_doub_array(splitted);
	if (!is_found)
	{
		if (is_directory(cmds->name))
			compound_error_exit(cmds, env, 126, ": Is a directory\n");
		else if (does_exist(cmds->name) && !can_access(cmds->name))
			compound_error_exit(cmds, env, 126, ": Permission denied\n");
		else if (does_exist(cmds->name))
			execve(cmds->name, cmds->args, env->environ);
	}
	compound_error_exit(cmds, env, 127, ": command not found\n");
}
