#include "push_swap.h"

/* implemet the function push */
/*
**	explain parameter
**	head_from = start stack
**	head_to = destination stack
**	tmp = temporary (to stability)
*/
void	push(t_node **stack_to, t_node **stack_from)
{
	t_node	*head_from;
	t_node	*head_to;
	t_node	*tmp;

	if (ft_lstsize(*stack_from) == 0)
		return ;
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
}

/* doing the function push a stack */
void	pa(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) == 0)
		return ;
	push(a, b);
	ft_printf("pa\n");
}

/* doing the function push b stack */
void	pb(t_node **a, t_node **b)
{
	if (ft_lstsize(*b) == 0)
		return ;
	push(b, a);
	ft_printf("pb\n");
}

