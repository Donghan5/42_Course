/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:42:23 by donghank          #+#    #+#             */
/*   Updated: 2024/08/16 12:17:30 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* includes */
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdio.h>
# include <stddef.h>
# include <string.h>
# include <stdint.h>
# include <limits.h>
/*========== define the buf size ==========*/
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/*==================== mandatory ====================*/
int			ft_atoi(const char *str);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);
void		ft_putchar_fd(char c, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(const char *str, int fd);
void		ft_putendl_fd(char const *s, int fd);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(const char *src);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *str, const char *to_find, size_t len);
void		*ft_memset(void *dest, int value, size_t count);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t count, size_t size);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t len);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_itoa(int n);
char		**ft_split(char const *s, char c);
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);

/*==================== printf ====================*/
int			ft_printf(const char *fmt, ...);
int			ft_putstr(const char *s);
int			ft_putchar(char c);
int			ft_putnbr(int nb);
int			ft_putnbr_base(unsigned int nb, const char *base);
int			ft_printf_address(void *fmt);
int			ft_printf_unsigned(unsigned int nb);
/*==================== get_next_line ====================*/
char		*get_next_line(int fd);
#endif
