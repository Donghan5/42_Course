# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 12:16:18 by donghank          #+#    #+#              #
#    Updated: 2024/08/04 14:50:57 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS = pipex_bonus

CC = cc
CFLAGS = -Werror -Wall -Wextra -I./includes -I./libft

RM = rm -rf

SRC_DIR = srcs
BONUS_DIR = srcs_bonus

SRCS = $(wildcard $(SRC_DIR)/*.c) libft/libft.a
SRCS_BONUS = $(wildcard $(BONUS_DIR)/*.c) libft/libft.a

# Rule to build the NAME target
$(NAME): $(SRCS)
	make -C libft
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

# Rule to build the BONUS target
$(BONUS): $(SRCS_BONUS)
	make -C libft
	$(CC) $(CFLAGS) $(SRCS_BONUS) -o $(BONUS)

# Default rule to build everything
all: $(NAME)

# Rule to clean built files
clean:
	$(RM) $(NAME) $(BONUS)
	make clean -C libft

# Rule to force clean and build everything again
fclean: clean
	$(RM) $(NAME) $(BONUS)
	make fclean -C libft

# Rule to rebuild everything
re: fclean all

# Rule to build the bonus target
bonus: $(BONUS)

.PHONY: all clean fclean re bonus
