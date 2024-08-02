/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:20 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 11:35:21 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
	The top element of the stack is sent to the bottom
	The reason we use double pointer to modify the head node
	tmp = temporary for the stability
	last = last node of the list.
*/
int	rotate(t_node **stack)
{
	t_node	*head;
	t_node	*last;

	if (ft_lstsize(*stack) < 2)
		return (-1);
	head = *stack;
	last = ft_lstlast(head);
	*stack = head->next;
	head->next = NULL;
	last->next = head;
	return (0);
}

/* rotate stack a*/
int	ra(t_node **a)
{
	if (rotate(a) == -1)
		return (-1);
	ft_printf("ra\n");
	return (0);
}

/* rotate stack b */
int	rb(t_node **b)
{
	if (rotate(b) == -1)
		return (-1);
	ft_printf("rb\n");
	return (0);
}

/* rotate stack a and stack b in the same time */
int	rr(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) < 2 || ft_lstsize(*b) < 2)
		return (-1);
	rotate(a);
	rotate(b);
	ft_printf("rr\n");
	return (0);
}
