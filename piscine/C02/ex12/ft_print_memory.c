/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 07:33:46 by donghank          #+#    #+#             */
/*   Updated: 2024/03/30 13:57:29 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < size)
	{
		write(1, &str[i], 1);
		i++;
	}
}

void	ft_print_hexa(char c)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (c > 16)
	{
		write(1, &hex[c / 16], 1);
		write(1, &hex[c % 16], 1);
	}
	else
	{
		write(1, "0", 1);
		write(1, &hex[(int)c], 1);
	}
}

void	print_address(unsigned long address, unsigned int count)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (address != 0)
	{
		print_address(address / 16, count + 1);
		write(1, &hex[address % 16], 1);
	}
	else
	{
		while (count++ < 16)
		{
			write(1, "0", 1);
		}
	}
}

int	is_printable(char *str)
{
	if (*str >= 32 && *str <= 126)
		return (1);
	else
		return (0);
}

void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	i;
	char			*arr;
	int				res;

	i = 0;
	arr = (char *)addr;
	res = is_printable(arr);
	if (size == 0)
		return (addr);
	print_address((unsigned long)addr, 0);
	write(1, ":", 1);
	while (i < size)
	{
		if ((i % 2) == 0)
			write(1, " ", 1);
		if (res == 1)
			ft_print_hexa(arr[i]);
		if (res == 0)
			write(1, ".", 1);
		i++;
	}
	addr = arr;
	write(1, " ", 1);
	ft_putstr(arr, size);
	return (addr);
}
