#include "push_swap.h"

/*
	reverse rotate
	head = head pointer of the stack
	last = last node of the stack
*/
void	reverse_rotate(t_node **stack)
{
	t_node	*head;
	t_node	*last;

	if (ft_lstsize(*stack) < 2)
		return ;
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
}

/* reverse rotate in the level of the stack a */
void	rra(t_node **a)
{
	reverse_rotate(a);
	ft_printf("rra\n");
}

/* reverse rotate in the level of the stack b */
void	rrb(t_node **b)
{
	reverse_rotate(b);
	ft_printf("rrb\n");
}

/* reverse rotate in the level of the stack a and b same time */
void	rrr(t_node **a, t_node **b)
{
	reverse_rotate(a);
	reverse_rotate(b);
	ft_printf("rrr\n");
}
