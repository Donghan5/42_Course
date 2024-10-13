/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_non_printable.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 07:19:20 by donghank          #+#    #+#             */
/*   Updated: 2024/03/30 12:30:41 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	print_hex(char c)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (c > 16)
	{
		ft_putchar(hex[(c / 16)]);
		ft_putchar(hex[(c % 16)]);
	}
	else
	{
		ft_putchar('0');
		ft_putchar(hex[(int)c]);
	}
}

void	ft_put_str_non_printable(char *str)
{	
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 32 && str[i] <= 126)
		{
			ft_putchar(str[i]);
		}
		else
		{
			ft_putchar('\\');
			print_hex(str[i]);
		}
		i++;
	}
}
