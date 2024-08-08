# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/02 11:36:26 by donghank          #+#    #+#              #
#    Updated: 2024/08/08 18:20:04 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

OBJDIR = objs
INCDIR = includes libft
SRCDIR = srcs
UTILDIR = utils
INCLUDES = $(addprefix -I, $(INCDIR))

SRC = srcs/main.c srcs/push.c srcs/radix_sort.c srcs/reverse_rotate.c \
	srcs/rotate.c srcs/sort.c srcs/swap.c \
	utils/cmd_utils.c utils/ft_lstutils.c utils/index.c utils/sort_utils.c \
	utils/utils.c
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

TARGET = push_swap

all: $(OBJDIR) libft $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -Llibft -lft -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(UTILDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: libft/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libft:
	$(MAKE) -C libft

clean:
	rm -rf $(OBJDIR)
	$(MAKE) clean -C libft

fclean: clean
	rm -rf $(TARGET)
	$(MAKE) fclean -C libft

re: fclean all

$(OBJDIR):
	@mkdir -p $(OBJDIR)

.PHONY: all clean fclean re libft
