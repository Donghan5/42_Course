/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ten_queens_puzzle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 07:32:58 by donghank          #+#    #+#             */
/*   Updated: 2024/03/25 12:36:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	to_pos(int n)
{
	if (n < 0)
		n *= -1;
	return (n);
}

int	ft_ten_queens_puzzle_check(int *board, int row)
{
	int	i;

	i = 0;
	while (i < row)
	{
		if (board[i] == board[row])
		{
			return (0);
		}
		else if (to_pos((board[i] - board[row])) == row - i)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_print_board(int *board)
{
	int		i;
	char	c;

	i = 0;
	while (i < 10)
	{
		c = board[i] + '0';
		write(1, &c, 1);
		i++;
	}
}

int	ft_solve_ten_queens(int *board, int row)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (row == 10)
	{
		ft_print_board(board);
		write(1, "\n", 1);
		return (1);
	}
	while (i < 10)
	{
		board[row] = i;
		if (ft_ten_queens_puzzle_check(board, row))
			count += ft_solve_ten_queens(board, row + 1);
		i++;
	}
	return (count);
}

int	ft_ten_queens_puzzle(void)
{
	int	board[10];

	return (ft_solve_ten_queens(board, 0));
}
