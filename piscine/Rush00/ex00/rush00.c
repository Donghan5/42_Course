/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush00.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edervise <edervise@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 16:17:42 by edervise          #+#    #+#             */
/*   Updated: 2024/03/17 13:19:09 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

void	ft_negative_input_management(int *a, int *b)
{
	if (*a < 0 && *b < 0)
	{
		*a *= (-1);
		*b *= (-1);
	}
	else if (*a < 0)
		*a *= (-1);
	else if (*b < 0)
		*b *= (-1);
}

void	rush(int a, int b)
{
	int	i;
	int	j;

	j = 1;
	ft_negative_input_management(&a, &b);
	while (j <= b)
	{
		i = 1;
		while (i <= a)
		{
			if (((j == 1) || (j == b)) && ((i == 1) || (i == a)))
				ft_putchar('o');
			else if ((j == 1) || (j == b))
				ft_putchar('-');
			else if ((i == 1) || (i == a))
				ft_putchar('|');
			else
				ft_putchar(' ');
			i++;
		}
		ft_putchar('\n');
		j++;
	}
}
