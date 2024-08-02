# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/12 23:26:15 by donghank          #+#    #+#              #
#    Updated: 2024/08/02 23:39:01 by donghank         ###   ########.fr        #
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

# Source and Object Files
SRC = $(wildcard $(SRCDIR)/*.c) $(wildcard libft/*.c)
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

# Output
TARGET = so_long

# Default target
all: $(TARGET)

# Link target depending on the OS
$(TARGET): $(OBJ)
ifeq ($(UNAME_S), Linux)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LINUX_FLAGS)
else ifeq ($(UNAME_S), Darwin)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(MACOS_FLAGS)
else
	$(error OS not supported)
endif

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: libft/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files
clean:
	rm -rf $(OBJDIR)

# Clean up object files and executable
fclean: clean
	rm -f $(TARGET)

# Rebuild
re: fclean all

.PHONY: all clean fclean re
