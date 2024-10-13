CC = cc
CFLAGS = -Wall -Werror -Wextra
SRCS = srcs/ft_putchar.c srcs/ft_swap.c srcs/ft_putstr.c srcs/ft_strlen.c srcs/ft_strcmp.c
INCS = includes
RM = rm -f
OBJS = ${SRCS : .c = .o}
LIBC = ar rc
LIBR = ranlib
NAME = libft.a

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<: .c = .o} -I ${INCS}

${NAME}: ${OBJS}
	${LIBC} ${NAME} ${OBJS}
	${LIBR} ${NAME}

all : ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all
