# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/12 23:26:15 by donghank          #+#    #+#              #
#    Updated: 2024/08/10 12:47:42 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler
CC = cc

# Flags
CFLAGS = -Wall -Wextra -Werror
LINUX_FLAGS = -L./mlx_linux -lmlx -lXext -lX11
MACOS_FLAGS = -L./mlx_mac -lmlx -framework OpenGL -framework AppKit

# Verify OS
UNAME_S := $(shell uname -s)

# Directories
SRCDIR = srcs
OBJDIR = objs
INCDIR = includes libft

# Include directories with -I prefix
INCLUDES = $(addprefix -I, $(INCDIR))

# Source Files
SRC =	$(SRCDIR)/draw_img.c \
		$(SRCDIR)/init.c \
		$(SRCDIR)/main.c \
		$(SRCDIR)/map.c \
		$(SRCDIR)/path_player.c \
		$(SRCDIR)/player_move.c \
		$(SRCDIR)/so_long.c \
		$(SRCDIR)/util_map.c

# Object Files
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

# Output
TARGET = so_long

# Default target
all: mlx libft $(TARGET)

# Link target depending on the OS
$(TARGET): $(OBJ)
ifeq ($(UNAME_S), Linux)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LINUX_FLAGS) -L./libft -lft
else ifeq ($(UNAME_S), Darwin)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(MACOS_FLAGS) -L./libft -lft
else
	$(error OS not supported)
endif

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Ensure libft is compiled
libft:
	$(MAKE) -C libft

mlx:
ifeq ($(UNAME_S), Linux)
	$(MAKE) -C mlx_linux
else ifeq ($(UNAME_S), Darwin)
	$(MAKE) -C mlx_mac
else
	$(error OS not supported)
endif

# Clean up object files
clean:
	rm -rf $(OBJDIR)
	$(MAKE) clean -C libft

# Clean up object files and executable
fclean: clean
	rm -f $(TARGET)
	$(MAKE) fclean -C libft
ifeq ($(UNAME_S), Linux)
	$(MAKE) clean -C mlx_linux
else ifeq ($(UNAME_S), Darwin)
	$(MAKE) clean -C mlx_mac
else
	$(error OS not supported)
endif

# Rebuild
re: fclean all

.PHONY: all clean fclean re libft mlx
