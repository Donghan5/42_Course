/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:29:29 by donghank          #+#    #+#             */
/*   Updated: 2024/06/06 23:19:27 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>

# define OPEN_MAX 8192
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*gnl_strchr(char *s, int c);
char	*gnl_strdup(char *s);
char	*gnl_strjoin(char *s1, char *s2);
size_t	gnl_strlen(char *s);
char	*gnl_substr(char *s, unsigned int start, size_t len);
#endif
