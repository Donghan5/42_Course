/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:34:59 by donghank          #+#    #+#             */
/*   Updated: 2024/08/14 13:36:25 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
**	implemet the function push
**	explain parameter
**	head_from = start stack
**	head_to = destination stack
**	tmp = temporary (to stability)
*/
int	push(t_node **stack_to, t_node **stack_from)
{
	t_node	*head_from;
	t_node	*head_to;
	t_node	*tmp;

	if (ft_lstsize(*stack_from) == 0)
		return (-1);
	head_from = *stack_from;
	head_to = *stack_to;
	tmp = head_from;
	head_from = head_from->next;
	*stack_from = head_from;
	if (!head_to)
	{
		head_to = tmp;
		head_to->next = NULL;
		*stack_to = head_to;
	}
	else
	{
		tmp->next = head_to;
		*stack_to = tmp;
	}
	return (0);
}

/* doing the function push a stack */
int	pa(t_node **a, t_node **b)
{
	if (push(a, b) == -1)
		return (-1);
	ft_printf("pa\n");
	return (0);
}

/* doing the function push b stack */
int	pb(t_node **a, t_node **b)
{
	if (push(b, a) == -1)
		return (-1);
	ft_printf("pb\n");
	return (0);
}
