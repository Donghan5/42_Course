# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 12:16:18 by donghank          #+#    #+#              #
#    Updated: 2024/07/27 14:11:11 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc

CFLAGS = -Werror -Wall -Wextra -I./includes -I./libft

RM = rm -rf

SRCS = 	srcs/pipex.c srcs/utils.c srcs/parsing.c srcs/main.c\
		libft/libft.a\

$(NAME) :
	make all -C libft
	cc $(CFLAGS) $(SRCS) -o $(NAME)


all : $(NAME)

fclean : clean
	$(RM) $(NAME)
	make fclean -C libft

clean :
	$(RM) $(NAME)
	make clean -C libft

re : fclean all

.PHONY : all clean fclean re
