/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:27 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 11:35:28 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* implement the function swap */
/*
	explain parameter
	tmp_value = to swap value (stability)
	tmp_index = to swap index (stability)
	head = to point the first node
	next = to point the second node
*/
int	swap(t_node **stack)
{
	int		tmp_value;
	int		tmp_index;
	t_node	*head;
	t_node	*next;

	if (ft_lstsize(*stack) < 2)
		return (-1);
	head = *stack;
	next = head->next;
	if (!head && !next)
		ft_printf("Swaping error\n");
	tmp_value = head->value;
	tmp_index = head->index;
	head->value = next->value;
	head->index = next->index;
	next->value = tmp_value;
	next->index = tmp_index;
	return (0);
}

/* swap the two first element in stack a */
int	sa(t_node **a)
{
	if (swap(a) == -1)
		return (-1);
	ft_printf("sa\n");
	return (0);
}

/* swap the two first element in stack b */
int	sb(t_node **b)
{
	if (swap(b) == -1)
		return (-1);
	ft_printf("sb\n");
	return (0);
}

/* swap the two first element in stack b */
int	ss(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) < 2 || ft_lstsize(*b) < 2)
		return (-1);
	swap(a);
	swap(b);
	ft_printf("ss\n");
	return (0);
}
