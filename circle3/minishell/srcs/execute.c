/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:04 by donghank          #+#    #+#             */
/*   Updated: 2024/09/26 14:42:14 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_fds(t_glob_pipe *glob_pipe)
{
	int	i;

	i = 0;
	while (i < glob_pipe->close_count)
	{
		close(glob_pipe->files_to_close[i]);
		i++;
	}
}

void	compound_error_exit(t_glob_pipe *cmds, t_env *env, int exit_code, char *msg)
{
	write(2, cmds->name, ft_strlen(cmds->name));
	if ((exit_code == 127) && (cmds->name[0] == '/' || ((cmds->name[0] == '.') && (cmds->name[1] == '/')) ))
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(msg, 2);
	free_glob_pipe(&cmds);
	free_doub_array(env->environ);
	rl_clear_history();
	exit(exit_code);
}

int	can_access(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	if (access(path, W_OK) == 0)
		return (1);
	if (access(path, R_OK) == 0)
		return (1);
	return (0);
}

int	does_exist(char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) == 0)
		return (1);
	return (0);
}

int	is_directory(char *path)
{
	DIR	*dir;

	if (*path != '/' && *path != '.')
		return (0);
	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

void search_path_and_run(t_glob_pipe *cmds, t_env *env)
{
    char		*path;
    char		**splitted;
    char		*full_path;
    int			i;
	struct stat	sb;

    path = getenv("PATH");
	if (is_directory(cmds->name))
		compound_error_exit(cmds, env, 126, ": Is a directory\n");
	else if (does_exist(cmds->name) && !can_access(cmds->name))
		compound_error_exit(cmds, env, 126, ": Permission denied\n");
    else if (does_exist(cmds->name))
        execve(cmds->name, cmds->args, env->environ);
    else
    {
        splitted = ft_split(path, ':');
        i = 0;
        while (splitted && splitted[i])
        {
            full_path = triple_strjoin(splitted[i], "/", cmds->name);
            if (!full_path)
            {
				handle_errors(&cmds, splitted, NULL);
				free_doub_array(env->environ);
				exit_error("malloc");
            }
			if (is_directory(full_path))
				compound_error_exit(cmds, env, 126, ": Is a directory\n");
			if (does_exist(full_path) && !can_access(full_path))
				compound_error_exit(cmds, env, 126, ": Permission denied\n");
            if (does_exist(full_path) && can_access(full_path))
            {
				free_doub_array(splitted);
                execve(full_path, cmds->args, env->environ);
				free_doub_array(env->environ);
                exit_error("");
            }
            free(full_path);
            i++;
        }
        free_doub_array(splitted);
    }
	compound_error_exit(cmds, env, 127, ": command not found\n");
}
