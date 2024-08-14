/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:58 by donghank          #+#    #+#             */
/*   Updated: 2024/08/14 13:36:47 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_free_stack(t_node **stack)
{
	t_node	*head;
	t_node	*tmp;

	head = *stack;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	free(stack);
}

/* to free the double pointer */
void	free_dptr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

/* to aide the free in the two stacks */
void	double_free(t_node *stack1, t_node *stack2)
{
	ft_printf("Fail to allocate.\n");
	free(stack1);
	free(stack2);
	return ;
}

/**
**	modify the function atoi to check argument
**/
int	ft_atoll(const char *str)
{
	int				sign;
	int				digit_check;
	long long int	result;

	result = 0;
	sign = 1;
	digit_check = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
		digit_check++;
	}
	if (*str != '\0' || (sign * result) > 2147483647 || \
	(sign * result) < -2147483648 || digit_check == 0)
		return (error_with_exit("Over the range or not a digit"), 0);
	return ((int)(sign * result));
}

/*
**	to economize the line of the function
*/
void	error_with_exit(char *msg)
{
	ft_printf("[ERROR]: %s\n", msg);
	exit(EXIT_FAILURE);
}
