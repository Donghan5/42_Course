# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/20 12:36:36 by donghank          #+#    #+#              #
#    Updated: 2024/08/21 10:48:25 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBC =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c \
		ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c \
		ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c \
		ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c

ADDITIONAL =	ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
				ft_split.c ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_substr.c ft_striteri.c \
				get_next_line.c

BONUS =	ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
		ft_lstdelone.c ft_lstiter.c ft_lstlast.c \
		ft_lstmap.c ft_lstnew.c ft_lstsize.c

PRINTF =	ft_printf.c ft_printf_address.c ft_printf_nbr.c ft_printf_strings.c

SRCS = ${LIBC} ${ADDITIONAL} ${PRINTF}

SRCSALL = ${LIBC} ${ADDITIONAL} ${BONUS} ${PRINTF}

OBJS = ${SRCS:.c=.o}

OBJSALL = ${SRCSALL:.c=.o}

NAME = libft.a

CC = cc

CFLAGS = -Wall -Werror -Wextra -g3 -I.

%.o: %.c
		${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
		ar -rcs ${NAME} ${OBJS}

bonus: ${OBJSALL}
		ar -rcs ${NAME} ${OBJSALL}

all: ${NAME}

clean:
		rm -f ${OBJS} ${OBJSALL}

fclean: clean
		rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re bonus
