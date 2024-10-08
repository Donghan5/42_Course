/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:27:45 by donghank          #+#    #+#             */
/*   Updated: 2024/09/09 15:24:11 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

/*---------- includes ----------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

/*---------- defines ----------*/
# define MSG_INFILE_ERROR "Fail to open infile"
# define MSG_OUTFILE_ERROR "Fail to open outfile"
# define MSG_EXEC_ERROR "Fail to exec"
# define MSG_DUP2_ERROR "Fail to dup2"
# define USAGE "Usage: ./piepx here_doc LIMITER cmd1 cmd2 outfile"
# define MSG_HEREDOC_ERROR "Fail to open .heredoc_tmp"
# define MSG_PIPE "Fail to create pipe"
# define FAIL 1
# define SUCCESS 0

/*---------- struct for pipex ----------*/
typedef struct s_pipex
{
	pid_t	*pids;
	char	*path;
	char	*cmd;
	char	**paths;
	char	**cmd_args;
	int		infile;
	int		outfile;
	int		**tubes;
	int		heredoc;
	int		start;
	int		limit;
}	t_pipex;

/*---------- parsing_bonus.c ----------*/
char	**get_paths(char **envp);
char	*find_path(t_pipex	*pipex, char **envp);
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index);

/*---------- pipex_bonus.c ----------*/
void	init_pipex(t_pipex *pipex, int fd1, int fd2, int cmd_count);
int		child_process(t_pipex *pipex, char **argv, char **envp, int cmd_index);
void	parent_process(t_pipex *pipex);
void	ft_pipex(t_pipex *pipex, char **argv, char **envp, int cmd_index);

/*---------- utils_bonus.c ----------*/
void	free_cmd_args(char **cmd_args);
void	free_paths(char **paths);
void	handle_error(char *str);
void	cleanup(t_pipex *pipex);
int		ft_strarray_len(char **arr);
void	handle_error_cleanup(t_pipex *pipex, char *msg);

/*---------- heredoc_bonus.c ----------*/
void	doing_process(t_pipex *pipex, char **argv, char **envp, int cmd_count);
#endif
