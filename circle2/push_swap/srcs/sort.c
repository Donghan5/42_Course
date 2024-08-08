/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:23 by donghank          #+#    #+#             */
/*   Updated: 2024/08/08 13:38:30 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
**	get minimal element in the stack
**	head = head pointer of the stack
**	min = the minimal value in the stack
*/
static int	get_min_element(t_node **stack, int val)
{
	t_node	*head;
	int		min;

	head = *stack;
	min = head->index;
	while (head->next)
	{
		head = head->next;
		if ((head->index < min) && head->index != val)
			min = head->index;
	}
	return (min);
}

/*
*	sort three elemet in the stack
*	val1 = the value of first
*	val2 = the value of second
*	val3 = the value of third
*	stack = want to swap
*/
static void	sort_three_element(t_node **a)
{
	int	val1;
	int	val2;
	int	val3;

	val1 = (*a)->value;
	val2 = (*a)->next->value;
	val3 = (*a)->next->next->value;
	if (is_sorted(a))
		return ;
	else if (val1 > val2 && val1 < val3)
		sa(a);
	else if (val1 < val2 && val2 > val3 && val1 < val3)
	{
		sa(a);
		ra(a);
	}
	else if (val1 > val2 && val2 > val3)
	{
		sa(a);
		rra(a);
	}
	else if (val1 < val2 && val2 > val3 && val1 > val3)
		rra(a);
	else if (val1 > val2 && val2 < val3 && val1 > val3)
		ra(a);
}

/*
**	sorting four element
**	distance = distance index (to serach right place)
*/
static void	sort_four_element(t_node **a, t_node **b)
{
	int	distance;

	if (is_sorted(a))
		return ;
	distance = get_distance_index(a, get_min_element(a, -1));
	if (distance == 1)
		ra(a);
	else if (distance == 2)
	{
		ra(a);
		ra(a);
	}
	else if (distance == 3)
		rra(a);
	if (is_sorted(a))
		return ;
	pb(a, b);
	sort_three_element(a);
	pa(a, b);
}

/*
**	do sort five element
**	distance = distance of the index(to right space)
*/
void	sort_five_element(t_node **a, t_node **b)
{
	int	distance;

	distance = get_distance_index(a, get_min_element(a, -1));
	if (distance == 1)
		ra(a);
	if (distance == 2)
	{
		ra(a);
		ra(a);
	}
	if (distance == 3)
	{
		rra(a);
		rra(a);
	}
	if (distance == 4)
		rra(a);
	if (is_sorted(a))
		return ;
	pb(a, b);
	sort_four_element(a, b);
	pa(a, b);
}

/*
**	do simple sort
**	lstsize = size of the stack a
*/
void	simple_sorting(t_node **a, t_node **b)
{
	int	lstsize;

	lstsize = ft_lstsize(*a);
	if (is_sorted(a) || lstsize == 0 || lstsize == 1)
		return ;
	if (lstsize == 2)
		sa(a);
	if (lstsize == 3)
		sort_three_element(a);
	if (lstsize == 4)
		sort_four_element(a, b);
	if (lstsize == 5)
		sort_five_element(a, b);
}
