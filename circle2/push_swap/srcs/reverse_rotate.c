/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:15 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 11:35:17 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
	reverse rotate
	head = head pointer of the stack
	last = last node of the stack
*/
int	reverse_rotate(t_node **stack)
{
	t_node	*head;
	t_node	*last;

	if (ft_lstsize(*stack) < 2)
		return (-1);
	head = *stack;
	last = ft_lstlast(head);
	while (head)
	{
		if (head->next->next == NULL)
		{
			head->next = NULL;
			break ;
		}
		head = head->next;
	}
	last->next = *stack;
	*stack = last;
	return (0);
}

/* reverse rotate in the level of the stack a */
int	rra(t_node **a)
{
	if (reverse_rotate(a) == -1)
		return (-1);
	ft_printf("rra\n");
	return (0);
}

/* reverse rotate in the level of the stack b */
int	rrb(t_node **b)
{
	if (reverse_rotate(b) == -1)
		return (-1);
	ft_printf("rrb\n");
	return (0);
}

/* reverse rotate in the level of the stack a and b same time */
int	rrr(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) < 2 || ft_lstsize(*b) < 2)
		return (-1);
	reverse_rotate(a);
	reverse_rotate(b);
	ft_printf("rrr\n");
	return (0);
}
