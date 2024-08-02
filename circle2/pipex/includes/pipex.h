/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:53:29 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 21:18:28 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/*---------- includes ----------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

/*---------- struct for pipex ----------*/
typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	char	*path;
	char	*cmd;
	char	**paths;
	char	**cmd_args;
	int		infile;
	int		outfile;
	int		tube[2];
}	t_pipex;

/*---------- parsing.c ----------*/
char	**get_path_from_env(char **envp);
char	*find_excutable_path(t_pipex *pipex, char **paths);
char	*find_path(t_pipex	*pipex, char **envp);
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index);

/*---------- pipex.c ----------*/
void	init_pipex(t_pipex *pipex, int fd1, int fd2);
void	child_process(t_pipex *pipex, char **argv, char **envp);
void	parent_process(t_pipex *pipex, char **argv, char **envp);

/*---------- utils.c ----------*/
void	free_cmd_args(char **cmd_args);
void	free_paths(char **paths);
void	handle_error(char *str);
void	cleanup(t_pipex *pipex);

/*---------- main.c ----------*/
void	doing_process(t_pipex *pipex, char **argv, char **envp);
#endif
