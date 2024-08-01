#include "ft_printf.h"
#include "./libft/libft.h"

int	main(void)
{
	int	num;

	num = 42;
	printf("OG printf: %s\n", "dsd");
	ft_printf("My printf: %s\n", "dsd");
	printf("OG printf: %d\n", num);
	ft_printf("My printf: %d\n", num);
	printf("OG printf: %p\n", &num);
	ft_printf("My printf: %p\n", &num);
	printf("OG printf: %x\n", num);
	ft_printf("My printf: %x\n", num);
	printf("OG printf: %X\n", num);
	ft_printf("My printf: %X\n", num);
	printf("OG printf: %u\n", -1);
	ft_printf("My printf: %u\n", -1);
	printf("OG printf: %%\n");
	ft_printf("My printf: %%\n");
	printf("I want to do\n");
	ft_printf("I want to do");
	return (0);
}

// cc -Wall -Werror -Wextra *.c */*.c
