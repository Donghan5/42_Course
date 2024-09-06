# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 12:16:18 by donghank          #+#    #+#              #
#    Updated: 2024/09/06 21:58:40 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS = pipex_bonus

CC = cc
# This for linux
# CFLAGS = -Werror -Wall -Wextra -g3 -I./includes -I./libft
# This for the mac environnement
CFLAGS = -Werror -Wall -Wextra -g3 -fsanitize=address -I./includes -I./libft

RM = rm -rf

SRC_DIR = srcs
BONUS_DIR = srcs_bonus

SRCS = srcs/main.c srcs/parsing.c srcs/pipex.c srcs/utils.c
SRCS_BONUS = srcs_bonus/main_bonus.c srcs_bonus/parsing_bonus.c \
			srcs_bonus/pipex_bonus.c srcs_bonus/utils_bonus.c srcs_bonus/heredoc_bonus.c

LIBFT = libft/libft.a

# Rule to build the NAME target
$(NAME): $(SRCS) $(LIBFT)
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) -o $(NAME)

# Rule to build the BONUS target
$(BONUS): $(SRCS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(SRCS_BONUS) $(LIBFT) -o $(BONUS)

# Default rule to build everything
all: $(NAME)

# Rule to clean built files
clean:
	$(RM) $(NAME) $(BONUS)
	make clean -C libft

# Rule to force clean and build everything again
fclean: clean
	make fclean -C libft

# Rule to rebuild everything
re: fclean all

# Rule to build the libft library
$(LIBFT):
	make -C libft

# Rule to build the bonus target
bonus: $(BONUS)

.PHONY: all clean fclean re bonus
