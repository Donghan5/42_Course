# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: donghank <donghank@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/20 12:36:36 by donghank          #+#    #+#              #
#    Updated: 2024/08/09 17:29:45 by donghank         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Define the paths to the source files
LIBC = mem/ft_memchr.c mem/ft_memcmp.c mem/ft_memcpy.c \
       mem/ft_memmove.c mem/ft_memset.c utils/ft_atoi.c \
       utils/ft_bzero.c utils/ft_calloc.c utils/ft_isalnum.c \
       utils/ft_isalpha.c utils/ft_isascii.c utils/ft_isdigit.c \
       utils/ft_isprint.c str/ft_strchr.c str/ft_strdup.c \
       str/ft_strlcat.c str/ft_strlcpy.c str/ft_strlen.c \
       str/ft_strncmp.c str/ft_strnstr.c str/ft_strrchr.c \
       utils/ft_tolower.c utils/ft_toupper.c

ADDITIONAL = put/ft_putchar_fd.c put/ft_putendl_fd.c put/ft_putnbr_fd.c \
             put/ft_putstr_fd.c str/ft_split.c str/ft_strjoin.c \
             str/ft_strmapi.c str/ft_strtrim.c str/ft_substr.c \
             str/ft_striteri.c utils/get_next_line.c utils/ft_itoa.c

BONUS = bonus/ft_lstadd_back_bonus.c bonus/ft_lstadd_front_bonus.c \
        bonus/ft_lstclear_bonus.c bonus/ft_lstdelone_bonus.c bonus/ft_lstiter_bonus.c \
        bonus/ft_lstlast_bonus.c bonus/ft_lstmap_bonus.c bonus/ft_lstnew_bonus.c \
        bonus/ft_lstsize_bonus.c

PRINTF = printf/ft_printf.c printf/ft_printf_address.c \
         printf/ft_printf_nbr.c printf/ft_printf_strings.c

# List of source files for standard and bonus builds
SRCS = ${LIBC} ${ADDITIONAL} ${PRINTF}
SRCSALL = ${SRCS} ${BONUS}

# Corresponding object files
OBJS = ${SRCS:.c=.o}
OBJSALL = ${SRCSALL:.c=.o}

# Name of the static library
NAME = libft.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra -I.

# Rule to compile .c files to .o files
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

# Rule to create the static library
${NAME}: ${OBJS}
	ar -rcs ${NAME} ${OBJS}

# Rule to create the library with bonus features
bonus: ${OBJSALL}
	ar -rcs ${NAME} ${OBJSALL}

# Default rule to build the library
all: ${NAME}

# Clean up object files
clean:
	rm -f ${OBJS} ${OBJSALL}

# Clean up object files and the library
fclean: clean
	rm -f ${NAME}

# Rebuild the library
re: fclean all

# Specify non-file targets
.PHONY: all clean fclean re bonus
